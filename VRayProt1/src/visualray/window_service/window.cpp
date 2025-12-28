#include "../kernel.h"
#include "../vrpch.h"
#include "../logservice.h"

#include "../event_service/game_events.h"
#include "../event_service/keyboard_events.h"
#include "../event_service/mouse_events.h"

#include "window.h"

#include <GLFW/glfw3.h>

namespace vray {
//	static bool glfwInitialized = false;  // !!!

	void Window::tryInitialize() {
		window = glfwCreateWindow(	data.width, 
									data.height,
									data.title.c_str(), nullptr, nullptr);

		glfwSetErrorCallback([](int error, const char* description) {
			VR_ENGINE_LOGERROR(description);
		});

		glfwMakeContextCurrent((GLFWwindow*)window);
		glfwSetWindowUserPointer((GLFWwindow*)window, &data);
		setVsync(false);
		glfwSwapInterval(60);

		/* Window callbacks */
		glfwSetWindowSizeCallback((GLFWwindow*)window, [](GLFWwindow* window, int width, int height) {
			VR_ENGINE_LOGERROR("WINDOW SIZE");

			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			WindowResizeEvent evt(width, height);
			data->width = width;
			data->height = height;
			data->callback(evt);
		});

		glfwSetWindowCloseCallback((GLFWwindow*)window, [](GLFWwindow* window) {
			VR_ENGINE_LOGERROR("WINDOW SHOULD CLOSE!");

			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent evt(true);
			data->callback(evt);
		});

		/* Input callbacks */
		glfwSetKeyCallback((GLFWwindow*)window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			VR_ENGINE_LOGERROR("KEY");

			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action) {
			case GLFW_PRESS: {
				KeyPressedEvent evt(key, 0);
				data->callback(evt);
				break;
			}
			case GLFW_RELEASE: {
				KeyReleasedEvent evt(key);
				data->callback(evt);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent evt(key, 1);
				data->callback(evt);
				break;
			}
			default:
				break;
			}
		});

		glfwSetMouseButtonCallback((GLFWwindow*)window, [](GLFWwindow* window, int button, int action, int mods) {
			VR_ENGINE_LOGERROR("MOUSE BUTTON");

			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				MouseClickEvent evt(button);
				data->callback(evt);
				break;
			}
			case GLFW_RELEASE: {
				MouseReleasedEvent evt(button);
				data->callback(evt);
				break;
			}
			default:
				break;
			}
		});

		glfwSetCursorPosCallback((GLFWwindow*)window, [](GLFWwindow* window, double xPos, double yPos) {
			VR_ENGINE_LOGERROR("CURSOR POS");
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent evt(xPos, yPos);
			data.callback(evt);
		});

		glfwSetScrollCallback((GLFWwindow*)window, [](GLFWwindow* window, double xOffset, double yOffset) {
			VR_ENGINE_LOGERROR("SCROLL");
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent evt(xOffset, yOffset);
			data.callback(evt);
		});
	}

	void Window::terminate() {
		glfwDestroyWindow((GLFWwindow*)window);
	}

	Window::Window(const WindowParams& _params){
		data.title = _params.title;
		data.width = _params.width;
		data.height = _params.height;
		closed = false;

		tryInitialize();
	}

	Window::~Window() {
		VR_ENGINE_LOGINFO("Destroying window!");
		terminate();

		glfwTerminate();
		VR_ENGINE_LOGINFO("GLFW terminated");
	}

	Window* Window::create(const WindowParams& params) {
		return new Window(params);
	}

	std::string Window::getErrorLog() {
		const char* description;
		int code = glfwGetError(&description);
		std::string errorLog = "Window - no error";

		if (code != GLFW_NO_ERROR) {
			errorLog = std::format("GLFW Error: {0} - {1}", code, description);
		}

		return errorLog;
	}

	void Window::onUpdate() {
		glfwPollEvents();
		GameUpdateEvent evt;
	}

	void Window::setVsync(bool enabled) {
		vsyncEnabled = enabled;
		glfwSwapInterval(enabled);
	}

	void Window::swapBuffers() { glfwSwapBuffers((GLFWwindow*)window); }
}