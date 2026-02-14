#pragma once
#include "../kernel.h"
#include "../vrpch.h"
#include "../logservice.h"

namespace vray {

	
	enum EventType {
		KEY_PRESSED,
		KEY_RELEASED,
		MOUSE_MOVED,
		MOUSE_SCROLLED,
		MOUSE_CLICK,
		MOUSE_RELEASED,
		WINDOW_RESIZE,
		WINDOW_CLOSE,
		GAME_TICK,
		GAME_UPDATE,
		RENDER_STEP,
		LAST
	};

	enum class EventCategory : int {
		NO_CATEGORY,
		INPUT_EVENT = SETBIT(0),
		KEYBOARD_EVENT = SETBIT(1),
		MOUSE_EVENT = SETBIT(2),
		RENDER_EVENT = SETBIT(3),
		GAME_EVENT = SETBIT(4)
	};

	constexpr EventCategory operator&(EventCategory left, EventCategory right) {
		return (EventCategory)((int)left & (int)right);
	}

	constexpr EventCategory operator|(EventCategory left, EventCategory right) {
		return (EventCategory)((int)left | (int)right);
	}

	class EventDispatcher;

	class VRAYLIB Event {
		friend EventDispatcher;
	protected:
		bool handled = false;

	public:
		virtual EventType getType() const = 0;
		virtual EventCategory getCategories() const = 0;
		virtual std::string getName() const = 0;
		virtual void dump() const { VR_LOGINFO("Event dump: " + getName()); }

		inline bool IsIn(EventCategory category) const { return (bool)(getCategories() & category); }
	};

	class EventDispatcher {
		template <typename T> using EventCallback = std::function<bool(T&)>;
	private:
		Event& _event;

	public:
		EventDispatcher(Event& event_) : _event(event_) {}

		template <typename T>
		bool fire(EventCallback<T> callback) {
			if (_event.getType() == T::getStaticType()) {
				_event.handled = callback(dynamic_cast<T&>(_event));
				return true;
			}
			return false;
		}
	};

#define VR_DEFINE_EVENT_TYPE(EVENT_TYPE) static EventType getStaticType() { return EVENT_TYPE; }\
	virtual EventType getType() const override { return getStaticType(); }\
	virtual std::string getName() const override { return #EVENT_TYPE; }

#define VR_DEFINE_EVENT_CATEGORY(EVENT_CATEGORY) virtual EventCategory getCategories() const override { return EVENT_CATEGORY; }
}