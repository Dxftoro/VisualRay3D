#pragma once
#include "../kernel.h"
#include "window_service/window.h"

namespace vray {

	//class Game;
	//class VRAYLIB InputService {
	//	friend Game;
	//private:
	//	static Window* activeWindow;

	//	InputService() {}
	//	~InputService() {}
	//	InputService(const InputService&) = delete;
	//	InputService& operator=(const InputService&) = delete;

	//public:
	//	enum class CursorMode {
	//		NORMAL = 0x00034001,
	//		HIDDEN = 0x00034002,
	//		DISABLED = 0x00034003
	//	};

	//private:
	//	static CursorMode currentCursorMode;

	//public:
	//	static void init(Window* activeWindow);

	//	static bool keyPressed(const int& key);
	//	static bool mouseButtonPressed(const int& mouseButton);
	//	static double getMouseX();
	//	static double getMouseY();

	//	static void setMousePosition(double x, double y);
	//	static void setMouseOnCenter();

	//	static void setCursorMode(CursorMode mode);
	//	static CursorMode getCursorMode();

	//	static void cleanup();
	//};

	class VRAYLIB InputService {
	public:
		enum class CursorMode {
			NORMAL = 0x00034001,
			HIDDEN = 0x00034002,
			DISABLED = 0x00034003
		};

	private:
		Window* activeWindow;
		CursorMode currentCursorMode;

	public:
		InputService()
		:	activeWindow(nullptr), currentCursorMode(CursorMode::NORMAL) {}
		InputService(Window* _activeWindow)
		:	activeWindow(_activeWindow),
			currentCursorMode(CursorMode::NORMAL) {}
		~InputService();

		bool keyPressed(const int key);
		bool mouseButtonPressed(const int mouseButton);
		double getMouseX();
		double getMouseY();

		void setMousePosition(double x, double y);
		void setMouseOnCenter();

		void setCursorMode(CursorMode mode);
		CursorMode getCursorMode();
	};

}