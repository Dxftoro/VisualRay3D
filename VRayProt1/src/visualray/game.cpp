#include "vrpch.h"
#include "game.h"
#include "event_service/game_events.h"
#include "layer_service/imgui_layer.h"
#include "input_service/input_service.h"
#include "managers/resource_loader.h"

namespace vray {
	float Game::begTime = 0.0f;
	float Game::endTime = 0.0f;
	float Game::_deltaTime = 0.0f;

	Game::Game() : running(false) {
		if (!glfwInit()) {
			VR_ENGINE_LOGERROR("Can't initialize GLFW!");
			throw std::runtime_error("Can't initialize GLFW!");
		}

		window = Window::create(WindowParams());
		window->setEventCallback(
			std::bind(&Game::onEvent, this, std::placeholders::_1)
		);
		window->setVsync(true);

		InputService::init(window);
		//pushOverlay(new ImGuiLayer(window));

		VR_ENGINE_LOGINFO("Trying to load mesh...");

		std::ifstream fin("models\\teapot.obj");
		if (!fin) {
			VR_ENGINE_LOGERROR("Can't open file!");
			throw std::runtime_error("Can't open file!");
		}

		renderer = new Renderer(window/*, vertexArray1*/);

		MeshLoader loader;
		VertexArray* vertexArray1 = (VertexArray*)loader.load(fin);
		renderer->setVertexArray(vertexArray1);
		fin.close();

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

		while (running) {
			endTime = glfwGetTime();
			_deltaTime = endTime - begTime;
			begTime = endTime;

			this->update();

			renderer->clear();
			renderer->update(deltaTime());

			window->onUpdate();
			layerStack.update();

			GameTickEvent evt;
			window->swapBuffers();
		}
	}

	inline void Game::stop() { running = false; }

	bool Game::onWindowClosing(WindowCloseEvent& evt) {
		running = false;
		return true;
	}

	void Game::onEvent(Event& evt) {
		EventDispatcher dispatcher(evt);
		dispatcher.fire<WindowCloseEvent>(
			std::bind(&Game::onWindowClosing, this, std::placeholders::_1)
		);
		renderer->onEvent(evt);
		glfwSetCursorPos((GLFWwindow*)window->getHandlerPtr(), window->getWidth() / 2, window->getHeight() / 2);
	}

	float Game::deltaTime() { return _deltaTime; }
}