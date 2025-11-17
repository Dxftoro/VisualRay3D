#pragma once
#include "../kernel.h"
#include "window_service/window.h"

namespace vray {

	class Game;
	class VRAYLIB InputService {
		friend Game;
	private:
		static Window* activeWindow;
		static bool _mouseLocked;

		InputService() {}
		~InputService() {}
		InputService(const InputService&) = delete;
		InputService& operator=(const InputService&) = delete;

		static void tryLockMouse();

	public:
		static void init(Window* activeWindow);

		static bool keyPressed(const int& key);
		static bool mouseButtonPressed(const int& mouseButton);
		static double getMouseX();
		static double getMouseY();

		static void setMouseLocked(bool locked);
		static bool isMouseLocked();
	};

}