#pragma once
#include "../kernel.h"
#include "../vrpch.h"
#include "../logservice.h"
#include "../util/lsid.h"

namespace vray {
	
	class Eid : public lsid::Sid32 {
	public:
		consteval Eid(const char* input) : lsid::Sid32(input) {}
	};

	namespace events {
		constexpr auto KEY_PRESSED		= "EVT_KEY_PRESSED";
		constexpr auto KEY_RELEASED		= "EVT_KEY_RELEASED";
		constexpr auto MOUSE_MOVED		= "EVT_MOUSE_MOVED";
		constexpr auto MOUSE_SCROLLED	= "EVT_MOUSE_SCROLLED";
		constexpr auto MOUSE_CLICK		= "EVT_MOUSE_CLICK";
		constexpr auto MOUSE_RELEASED	= "EVT_MOUSE_RELEASED";
		constexpr auto WINDOW_RESIZE	= "EVT_WINDOW_RESIZE";
		constexpr auto WINDOW_CLOSE		= "EVT_WINDOW_CLOSE";
		constexpr auto COLLISION		= "EVT_COLLISION";
		constexpr auto TRIGGER			= "EVT_TRIGGER";
	};

	enum class EventCategory : int {
		NO_CATEGORY,
		INPUT_EVENT = SETBIT(0),
		KEYBOARD_EVENT = SETBIT(1),
		MOUSE_EVENT = SETBIT(2),
		RENDER_EVENT = SETBIT(3),
		PHYSICS_EVENT = SETBIT(4),
		GAME_EVENT = SETBIT(5)
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
		virtual Eid getType() const = 0;
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

#define VR_DEFINE_EVENT_TYPE(EVENT_TYPE) static constexpr Eid getStaticType() { return Eid(EVENT_TYPE); }\
	virtual Eid getType() const override { return getStaticType(); }\
	virtual std::string getName() const override { return #EVENT_TYPE; }

#define VR_DEFINE_EVENT_CATEGORY(EVENT_CATEGORY) virtual EventCategory getCategories() const override { return EVENT_CATEGORY; }
}