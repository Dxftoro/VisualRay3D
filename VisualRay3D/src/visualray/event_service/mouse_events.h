#pragma once
#include "../vrpch.h"
#include "../kernel.h"
#include "event.h"

namespace vray {

	class VRAYLIB MouseMovedEvent : public Event {
	private:
		double x, y;

	public:
		MouseMovedEvent(const double& _x, const double& _y) : x(_x), y(_y) {}

		double getX() const { return x; }
		double getY() const { return y; }

		void dump() const override {
			VR_LOGINFO("Event dump: " + getName() + " x " + std::to_string(x) + ", y " + std::to_string(y));
		}

		VR_DEFINE_EVENT_TYPE(EventType::MOUSE_MOVED)
		VR_DEFINE_EVENT_CATEGORY(EventCategory::INPUT_EVENT | EventCategory::MOUSE_EVENT)
	};

	class VRAYLIB MouseScrolledEvent : public Event {
	private:
		double xScrollOffset, yScrollOffset;

	public:
		MouseScrolledEvent(const double& _xScrollOffset, const double& _yScrollOffset)
			: xScrollOffset(_xScrollOffset), yScrollOffset(_yScrollOffset) {}

		double getXScrollOffset() const { return xScrollOffset; }
		double getYScrollOffset() const { return yScrollOffset; }

		void dump() const override {
			VR_LOGINFO("Event dump: " + getName() + 
				" xso: " + std::to_string(xScrollOffset) +
				" yso: " + std::to_string(yScrollOffset));
		}

		VR_DEFINE_EVENT_TYPE(EventType::MOUSE_SCROLLED)
		VR_DEFINE_EVENT_CATEGORY(EventCategory::INPUT_EVENT | EventCategory::MOUSE_EVENT)
	};

	class VRAYLIB MouseButtonEvent : public Event {
	protected:
		int mouseButtonCode;
		MouseButtonEvent(int _mouseButtonCode) : mouseButtonCode(_mouseButtonCode) {}

	public:
		int getMouseButtonCode() const { return mouseButtonCode; }

		VR_DEFINE_EVENT_CATEGORY(EventCategory::INPUT_EVENT | EventCategory::MOUSE_EVENT)
	};

	class VRAYLIB MouseClickEvent : public MouseButtonEvent {
	public:
		MouseClickEvent(int mouseButtonCode) : MouseButtonEvent(mouseButtonCode) {}

		void dump() const override {
			VR_LOGINFO("Event dump: " + getName() + " mbc: " + std::to_string(mouseButtonCode));
		}

		VR_DEFINE_EVENT_TYPE(EventType::MOUSE_CLICK)
	};

	class VRAYLIB MouseReleasedEvent : public MouseButtonEvent {
	public:
		MouseReleasedEvent(int mouseButtonCode) : MouseButtonEvent(mouseButtonCode) {}

		void dump() const override {
			VR_LOGINFO("Event dump: " + getName() + " mbc: " + std::to_string(mouseButtonCode));
		}

		VR_DEFINE_EVENT_TYPE(EventType::MOUSE_RELEASED)
	};
}