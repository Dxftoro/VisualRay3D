#include "vrpch.h"
#include "game.h"

#include <GLFW/glfw3.h>
#include <reactphysics3d/reactphysics3d.h>

#include "event_service/game_events.h"
#include "layer_service/imgui_layer.h"
#include "layer_service/debugger.h"
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

	Game::Game(const WindowParams& windowParams) : running(false) {
		if (glfwInit() == GLFW_FALSE) {
			VR_ENGINE_LOGERROR("Can't initialize GLFW!");
			throw std::runtime_error("Can't initialize GLFW!");
		}

		engineContext.window = std::unique_ptr<Window>(Window::create(windowParams));
		engineContext.window->setEventCallback(
			std::bind(&Game::onEventInternal, this, std::placeholders::_1)
		);

		engineContext.inputService = InputService(engineContext.window.get());
		engineContext.debugger = new Debugger(engineContext.window.get());
		//pushOverlay(new ImGuiLayer(engineContext.window.get()));

		visibleGroup = gameContext.world.group<CompTransform>(entt::get<CompRenderable>);
		
		engineContext.renderer = new Renderer(engineContext.window.get(), gameContext.world);
		Renderer* renderer = engineContext.renderer;

		engineContext.physics = new Rp3dPhysics(gameContext.world);
		IPhysics* physics = engineContext.physics;

		engineContext.physicsDebugSystem = new Rp3dDebugSystem(dynamic_cast<Rp3dPhysics*>(physics), renderer);
		engineContext.cameraSystem = CameraSystem(renderer);

		VR_ENGINE_LOGINFO((const char*)glGetString(GL_VENDOR));
		VR_ENGINE_LOGINFO((const char*)glGetString(GL_RENDERER));
		VR_ENGINE_LOGINFO((const char*)glGetString(GL_VERSION));
	}

	Game::~Game() {
		VR_ENGINE_LOGINFO("Clearing renderer");
		delete engineContext.renderer;
		VR_ENGINE_LOGINFO("Clearing physics");
		delete engineContext.physics;
		VR_ENGINE_LOGINFO("Clearing physics debug system");
		delete engineContext.physicsDebugSystem;
		VR_ENGINE_LOGINFO("Clearing debugger");
		delete engineContext.debugger;
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

			engineContext.physics->update(_deltaTime);
			if (frameNumber >= frameInterval) {
				engineContext.physicsDebugSystem->update(true);
				frameNumber = 0;
			}
			else frameNumber++;

			this->update();
			this->renderSubmit();

			engineContext.renderer->clear();
			engineContext.renderer->update(deltaTime());

			engineContext.window->onUpdate();
			//engineContext.layerStack.update();
			engineContext.debugger->update();
			engineContext.window->swapBuffers();

			std::this_thread::sleep_until(frameEnd);
		}
	}

	inline void Game::stop() { running = false; }

	bool Game::onWindowClosing(WindowCloseEvent& evt) {
		running = false;
		engineContext.window->setClosed(true);
		return true;
	}

	void Game::renderSubmit() {
		visibleGroup.each([this]
		(entt::entity entity, CompTransform& transform, CompRenderable& renderable) {
			RenderRequest request(&renderable, &transform, 4U);
			engineContext.renderer->submit(std::move(request));
		});
	}

	void Game::onEventInternal(Event& evt) {
		EventDispatcher dispatcher(evt);
		dispatcher.fire<WindowCloseEvent>(
			std::bind(&Game::onWindowClosing, this, std::placeholders::_1)
		);

		onEvent(evt);
		engineContext.renderer->onEvent(evt);
	}

	float Game::deltaTime() { return _deltaTime; }
}