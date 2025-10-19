#include "vrpch.h"
#include "../kernel.h"

#include "input_service.h"
#include "logservice.h"
#include <GLFW/glfw3.h>

namespace vray {

	Window* InputService::activeWindow = nullptr;

	void InputService::init(Window* _activeWindow) {
		if (activeWindow) {
			throw std::runtime_error("Active window is set in InputService!");
		}
		activeWindow = _activeWindow;
	}

	bool InputService::keyPressed(const int& key) {
		return glfwGetKey((GLFWwindow*)activeWindow->getHandlerPtr(), key);
	}

	bool InputService::mouseButtonPressed(const int& mouseButton) {
		return glfwGetMouseButton((GLFWwindow*)activeWindow->getHandlerPtr(), mouseButton);
	}

	double InputService::getMouseX() {
		double x, y;
		glfwGetCursorPos((GLFWwindow*)activeWindow->getHandlerPtr(), &x, &y);
		return x;
	}

	double InputService::getMouseY() {
		double x, y;
		glfwGetCursorPos((GLFWwindow*)activeWindow->getHandlerPtr(), &x, &y);
		return y;
	}

}