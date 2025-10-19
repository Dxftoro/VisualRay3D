#pragma once
#include "../vrpch.h"
#include "../kernel.h"
#include "event.h"

namespace vray {

	class VRAYLIB KeyEvent : public Event {
	protected:
		int keyCode;
		KeyEvent(int _keyCode) : keyCode(_keyCode) {}

	public:
		int getKeyCode() const { return keyCode; }

		VR_DEFINE_EVENT_CATEGORY(EventCategory::INPUT_EVENT | EventCategory::KEYBOARD_EVENT)
	};

	class VRAYLIB KeyPressedEvent : public KeyEvent {
		unsigned int repeatCount;
	public:
		KeyPressedEvent(int keyCode, unsigned int _repeatCount)
			: KeyEvent(keyCode), repeatCount(_repeatCount) {}

		unsigned int getPressCount() const { return repeatCount; }

		void dump() const override {
			VR_LOGINFO("Event dump: " + getName() + 
				" kc: " + std::to_string(keyCode) + 
				" pc: " + std::to_string(repeatCount));
		}

		VR_DEFINE_EVENT_TYPE(EventType::KEY_PRESSED)
	};

	class VRAYLIB KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

		void dump() const override {
			VR_LOGINFO("Event dump: " + getName() + " kc: " + std::to_string(keyCode));
		}

		VR_DEFINE_EVENT_TYPE(EventType::KEY_RELEASED)
	};

}