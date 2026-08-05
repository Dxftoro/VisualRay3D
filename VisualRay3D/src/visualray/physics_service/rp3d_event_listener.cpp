#include "vrpch.h"
#include "rp3d_event_listener.h"
#include "../event_service/physics_events.h"

namespace vray {

	void Rp3dEventListener::onContact(const rp3d::CollisionCallback::CallbackData& data) {
		for (rp3d::uint32 i = 0; i < data.getNbContactPairs(); ++i) {
			rp3d::CollisionCallback::ContactPair contactPair = data.getContactPair(i);

			CollisionEvent event(
				(entt::entity)(uintptr_t)contactPair.getBody1()->getUserData(),
				(entt::entity)(uintptr_t)contactPair.getBody2()->getUserData(),
				(CollisionEvent::ContactType)(int)contactPair.getEventType()
			);

			callback(event);
		}
	}

	void Rp3dEventListener::onTrigger(const rp3d::OverlapCallback::CallbackData& data) {
		for (rp3d::uint32 i = 0; i < data.getNbOverlappingPairs(); ++i) {
			rp3d::OverlapCallback::OverlapPair overlapPair = data.getOverlappingPair(i);

			TriggerEvent event(
				(entt::entity)(uintptr_t)overlapPair.getBody1()->getUserData(),
				(entt::entity)(uintptr_t)overlapPair.getBody2()->getUserData(),
				(CollisionEvent::ContactType)(int)overlapPair.getEventType()
			);

			callback(event);
		}
	}

}