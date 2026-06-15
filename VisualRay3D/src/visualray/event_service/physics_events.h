#pragma once
#include "kernel.h"
#include "event.h"
#include <thirdparty/EnTT/entt.hpp>

namespace vray {

	class VRAYLIB PhysicsEvent : public Event {
	public:
		enum class ContactType {
			START, STAY, STOP,
		};

	protected:
		entt::entity entity1, entity2;
		ContactType type;

	public:
		PhysicsEvent(entt::entity _entity1, entt::entity _entity2, ContactType _type)
			: entity1(_entity1), entity2(_entity2), type(_type) {
		}

		entt::entity getEntity1() const { return entity1; }
		entt::entity getEntity2() const { return entity2; }
		ContactType getContactType() const { return type; }

		VR_DEFINE_EVENT_CATEGORY(EventCategory::PHYSICS_EVENT)
	};

	class VRAYLIB CollisionEvent final : public PhysicsEvent {
	public:
		CollisionEvent(entt::entity entity1, entt::entity entity2, ContactType type)
			: PhysicsEvent(entity1, entity2, type) {
		}

		void dump() const override {
			VR_LOGINFO("Event dump: " + getName() +
				" entity1: " + std::to_string((uint32_t)entity1) +
				" entity2: " + std::to_string((uint32_t)entity2));
		}

		VR_DEFINE_EVENT_TYPE(EventType::COLLISION)
	};

	class VRAYLIB TriggerEvent final : public PhysicsEvent {
	public:
		TriggerEvent(entt::entity entity1, entt::entity entity2, ContactType type)
			: PhysicsEvent(entity1, entity2, type) {
		}

		void dump() const override {
			VR_LOGINFO("Event dump: " + getName() +
				" entity1: " + std::to_string((uint32_t)entity1) +
				" entity2: " + std::to_string((uint32_t)entity2));
		}

		VR_DEFINE_EVENT_TYPE(EventType::TRIGGER)
	};

}