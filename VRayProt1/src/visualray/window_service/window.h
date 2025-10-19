#pragma once
#include "../kernel.h"
#include "../event_service/event.h"

namespace vray {

	struct WindowParams {
		std::string title;
		int width, height;

		WindowParams()
			: title("VisualRay game"), width(1200), height(800) {}

		WindowParams(const std::string& _title, int _width, int _height)
			: title(_title), width(_width), height(_height) {}
	};

	class VRAYLIB Window {
		using EventCallback = std::function<void(Event& evt)>;
	private:
		struct WindowData {
			std::string title;
			int width = 0, height = 0;
			EventCallback callback;
		} data;

		void* window;
		bool vsyncEnabled;

		void tryInitialize();
		void terminate();

	public:
		Window(const WindowParams& params = WindowParams());
		~Window();

		static Window* create(const WindowParams& params);

		std::string getTitle() const { return data.title; };
		int getWidth() const { return data.width; };
		int getHeight() const { return data.height; };
		void* getHandlerPtr() const { return window; }

		void onUpdate();
		void setVsync(bool enabled);
		void setEventCallback(const EventCallback& callback) { data.callback = callback; }
		inline void swapBuffers();

		bool isVsyncEnabled() const { return vsyncEnabled; };
	};

}