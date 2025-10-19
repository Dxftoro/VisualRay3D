#pragma once
#include "../kernel.h"
#include "window_service/window.h"

namespace vray {

	class VRAYLIB InputService {
	private:
		static Window* activeWindow;

		InputService() {}
		~InputService() {}
		InputService(const InputService&) = delete;
		InputService& operator=(const InputService&) = delete;

	public:
		static void init(Window* activeWindow);

		static bool keyPressed(const int& key);
		static bool mouseButtonPressed(const int& mouseButton);
		static double getMouseX();
		static double getMouseY();
	};

}