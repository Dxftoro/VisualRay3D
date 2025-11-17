#include "vrpch.h"
#include "game.h"

#include <GLFW/glfw3.h>

#include "event_service/game_events.h"
#include "layer_service/imgui_layer.h"
#include "input_service/input_service.h"
#include "managers/resource_loader.h"
#include "render_service/render_request.h"

namespace vray {
	float Game::begTime = 0.0f;
	float Game::endTime = 0.0f;
	float Game::_deltaTime = 1.0f;
	int Game::fpsLimit = 30;

	Game::Game() : running(false), cameraSystem(nullptr) {
		if (!glfwInit()) {
			VR_ENGINE_LOGERROR("Can't initialize GLFW!");
			throw std::runtime_error("Can't initialize GLFW!");
		}

		window = Window::create(WindowParams());
		window->setEventCallback(
			std::bind(&Game::onEventInternal, this, std::placeholders::_1)
		);

		InputService::init(window);
		pushOverlay(new ImGuiLayer(window));

		VR_ENGINE_LOGINFO("Trying to load mesh...");

		std::ifstream fin("models\\teapot.obj");
		if (!fin) {
			VR_ENGINE_LOGERROR("Can't open file!");
			throw std::runtime_error("Can't open file!");
		}

		visibleGroup = world.group<CompTransform>(entt::get<CompRenderable>);
		renderer = new Renderer(window);
		cameraSystem = CameraSystem(renderer);

		VR_ENGINE_LOGINFO((const char*)glGetString(GL_VENDOR));
		VR_ENGINE_LOGINFO((const char*)glGetString(GL_RENDERER));
		VR_ENGINE_LOGINFO((const char*)glGetString(GL_VERSION));
	}

	Game::~Game() {
		delete renderer;
		glfwTerminate();
		VR_ENGINE_LOGINFO("GLFW terminated.");
	}

	inline void Game::run() {
		running = true;
		VR_ENGINE_LOGINFO("Game application started running!");

		begTime = glfwGetTime();

		while (running) {
			auto frameBegin = std::chrono::high_resolution_clock::now();
			auto frameEnd = frameBegin + std::chrono::milliseconds(1000 / fpsLimit);

			endTime = glfwGetTime();
			_deltaTime = endTime - begTime;
			begTime = endTime;

			this->update();
			this->renderSubmit();

			renderer->clear();
			renderer->update(deltaTime());

			window->onUpdate();
			layerStack.update();

			GameTickEvent evt;
			window->swapBuffers();

			std::this_thread::sleep_until(frameEnd);
		}
	}

	inline void Game::stop() { running = false; }

	bool Game::onWindowClosing(WindowCloseEvent& evt) {
		running = false;
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

		InputService::tryLockMouse();
		onEvent(evt);
		renderer->onEvent(evt);
	}

	float Game::deltaTime() { return _deltaTime; }
}