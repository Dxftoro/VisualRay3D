#pragma once
#include "../vrpch.h"
#include "../kernel.h"
#include "event.h"

namespace vray {

	class VRAYLIB WindowResizeEvent : public Event {
	private:
		int width, height;
	public:
		WindowResizeEvent(int _width, int _height)
			: width(_width), height(_height) {}

		int getWidth() const { return width; }
		int getHeight() const { return height; }

		void dump() const override {
			VR_LOGINFO("Event dump: " + getName() + " x " + std::to_string(width) + " y " + std::to_string(height));
		}

		VR_DEFINE_EVENT_TYPE(EventType::WINDOW_RESIZE)
		VR_DEFINE_EVENT_CATEGORY(EventCategory::GAME_EVENT)
	};

	class VRAYLIB WindowCloseEvent : public Event {
	private:
		bool shouldClose;
	public:
		WindowCloseEvent(bool _shouldClose)
			: shouldClose(_shouldClose) {}

		bool windowShouldClose() const { return shouldClose; }

		void dump() const override {
			VR_LOGINFO("Event dump: " + getName() + " " + std::to_string(shouldClose));
		}

		VR_DEFINE_EVENT_TYPE(EventType::WINDOW_CLOSE)
		VR_DEFINE_EVENT_CATEGORY(EventCategory::GAME_EVENT)
	};

	class VRAYLIB GameTickEvent : public Event {
	public:
		GameTickEvent() {}

		VR_DEFINE_EVENT_TYPE(EventType::GAME_TICK)
		VR_DEFINE_EVENT_CATEGORY(EventCategory::GAME_EVENT)
	};

	class VRAYLIB GameUpdateEvent : public Event {
	public:
		GameUpdateEvent() {}

		VR_DEFINE_EVENT_TYPE(EventType::GAME_UPDATE)
		VR_DEFINE_EVENT_CATEGORY(EventCategory::GAME_EVENT)
	};
}