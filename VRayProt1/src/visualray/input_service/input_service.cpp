#include "vrpch.h"
#include "../kernel.h"

#include "input_service.h"
#include "logservice.h"
#include <GLFW/glfw3.h>

namespace vray {

	InputService::~InputService() {
		VR_ENGINE_LOGINFO("Cleaning up input service");
		activeWindow = nullptr;
	}

	bool InputService::keyPressed(const int key) {
		return glfwGetKey((GLFWwindow*)activeWindow->getHandlerPtr(), key);
	}

	bool InputService::mouseButtonPressed(const int mouseButton) {
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

	void InputService::setMousePosition(double x, double y) {
		glfwSetCursorPos((GLFWwindow*)activeWindow, x, y);
	}

	void InputService::setMouseOnCenter() {
		glfwSetCursorPos(
			(GLFWwindow*)activeWindow->getHandlerPtr(),
			activeWindow->getWidth() / 2, activeWindow->getHeight() / 2
		);
	}

	void InputService::setCursorMode(CursorMode mode) {
		glfwSetInputMode((GLFWwindow*)activeWindow->getHandlerPtr(),
			GLFW_CURSOR, (int)mode);
		currentCursorMode = mode;
	}

	InputService::CursorMode InputService::getCursorMode() {
		return currentCursorMode;
	}

}