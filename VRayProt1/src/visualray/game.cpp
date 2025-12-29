#include "vrpch.h"
#include "game.h"

#include <GLFW/glfw3.h>
#include <reactphysics3d/reactphysics3d.h>

#include "event_service/game_events.h"
#include "layer_service/imgui_layer.h"
#include "managers/resource_loader.h"
#include "render_service/render_request.h"

//#include "physics_service/ode_physics.h"
#include "physics_service/rp3d_physics.h"
#include "physics_service/rp3d_debug_system.h"

namespace vray {
	float Game::begTime = 0.0f;
	float Game::endTime = 0.0f;
	float Game::_deltaTime = 1.0f;
	int Game::fpsLimit = 30;

	Game::Game(const WindowParams& windowParams) : running(false), cameraSystem(nullptr) {
		if (glfwInit() == GLFW_FALSE) {
			VR_ENGINE_LOGERROR("Can't initialize GLFW!");
			throw std::runtime_error("Can't initialize GLFW!");
		}

		window = std::unique_ptr<Window>(Window::create(windowParams));
		window->setEventCallback(
			std::bind(&Game::onEventInternal, this, std::placeholders::_1)
		);

		inputService = InputService(window.get());
		pushOverlay(new ImGuiLayer(window.get()));

		visibleGroup = world.group<CompTransform>(entt::get<CompRenderable>);
		renderer = new Renderer(window.get());

		physics = new Rp3dPhysics(world);
		physicsDebugSystem = new Rp3dDebugSystem(dynamic_cast<Rp3dPhysics*>(physics), renderer);
		cameraSystem = CameraSystem(renderer);

		VR_ENGINE_LOGINFO((const char*)glGetString(GL_VENDOR));
		VR_ENGINE_LOGINFO((const char*)glGetString(GL_RENDERER));
		VR_ENGINE_LOGINFO((const char*)glGetString(GL_VERSION));
	}

	Game::~Game() {
		VR_ENGINE_LOGINFO("Clearing renderer");
		delete renderer;
		VR_ENGINE_LOGINFO("Clearing physics");
		delete physics;
		VR_ENGINE_LOGINFO("Clearing physics debug system");
		delete physicsDebugSystem;
	}

	inline void Game::run() {
		running = true;
		VR_ENGINE_LOGINFO("Game application started running!");

		begTime = glfwGetTime();

		int frameInterval = 1, frameNumber = 0;
		while (running) {
			auto frameBegin = std::chrono::high_resolution_clock::now();
			auto frameEnd = frameBegin + std::chrono::milliseconds(1000 / fpsLimit);

			endTime = glfwGetTime();
			_deltaTime = endTime - begTime;
			begTime = endTime;

			physics->update(_deltaTime);
			if (frameNumber >= frameInterval) {
				physicsDebugSystem->update(true);
				frameNumber = 0;
			}
			else frameNumber++;

			this->update();
			this->renderSubmit();

			renderer->clear();
			renderer->update(deltaTime());

			window->onUpdate();
			layerStack.update();
			window->swapBuffers();

			std::this_thread::sleep_until(frameEnd);
		}
	}

	inline void Game::stop() { running = false; }

	bool Game::onWindowClosing(WindowCloseEvent& evt) {
		running = false;
		window->setClosed(true);
		return true;
	}

	void Game::renderSubmit() {
		visibleGroup.each([this]
		(entt::entity entity, CompTransform& transform, CompRenderable& renderable) {
			RenderRequest request(&renderable, &transform, 4U);
			renderer->submit(std::move(request));
		});
	}

	void Game::onEventInternal(Event& evt) {
		EventDispatcher dispatcher(evt);
		dispatcher.fire<WindowCloseEvent>(
			std::bind(&Game::onWindowClosing, this, std::placeholders::_1)
		);

		onEvent(evt);
		renderer->onEvent(evt);
	}

	float Game::deltaTime() { return _deltaTime; }
}