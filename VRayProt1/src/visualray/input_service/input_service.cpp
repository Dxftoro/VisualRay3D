#include "vrpch.h"
#include "../kernel.h"

#include "input_service.h"
#include "logservice.h"
#include <GLFW/glfw3.h>

#define VR_GLFW_WINDOW (GLFWwindow*)activeWindow->getHandlerPtr()

namespace vray {

	InputService::~InputService() {
		VR_ENGINE_LOGINFO("Cleaning up input service");
		activeWindow = nullptr;
	}

	bool InputService::keyPressed(const int key) {
		return glfwGetKey(VR_GLFW_WINDOW, key);
	}

	bool InputService::mouseButtonPressed(const int mouseButton) {
		return glfwGetMouseButton(VR_GLFW_WINDOW, mouseButton);
	}

	double InputService::getMouseX() {
		double x, y;
		glfwGetCursorPos(VR_GLFW_WINDOW, &x, &y);
		return x;
	}

	double InputService::getMouseY() {
		double x, y;
		glfwGetCursorPos(VR_GLFW_WINDOW, &x, &y);
		return y;
	}

	void InputService::setMousePosition(double x, double y) {
		glfwSetCursorPos(VR_GLFW_WINDOW, x, y);
	}

	void InputService::setMouseOnCenter() {
		glfwSetCursorPos(
			VR_GLFW_WINDOW,
			activeWindow->getWidth() / 2, activeWindow->getHeight() / 2
		);
	}

	void InputService::setCursorMode(CursorMode mode) {
		glfwSetInputMode(VR_GLFW_WINDOW, GLFW_CURSOR, (int)mode);
		currentCursorMode = mode;
	}

	void InputService::setRawMouseInputEnabled(bool enabled) {
		if (glfwRawMouseMotionSupported()) {
			glfwSetInputMode(VR_GLFW_WINDOW, GLFW_RAW_MOUSE_MOTION, enabled ? GLFW_TRUE : GLFW_FALSE);
			rawMouseInputEnabled = enabled;
		}
		else {
			VR_ENGINE_LOGERROR("Raw mouse input is not supported by this platform!");
		}
	}

	InputService::CursorMode InputService::getCursorMode() const {
		return currentCursorMode;
	}

	bool InputService::isRawMouseInputEnabled() const {
		return (glfwGetInputMode(VR_GLFW_WINDOW, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE);
	}

}