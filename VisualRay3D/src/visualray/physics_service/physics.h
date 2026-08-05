#pragma once
#include "../vrpch.h"
#include "raycast_result.h"
#include "../event_service/event.h"
#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/EnTT/entt.hpp"

namespace vray {

	class IPhysics {
	public:
		using EventCallback = std::function<void(Event&)>;
		using RaycastCallback = std::function<void(const RaycastResult&)>;

		virtual ~IPhysics() = default;

		virtual void update(float deltaTime) = 0;

		virtual std::optional<RaycastResult> raycastBack(const glm::vec3& start, const glm::vec3& end) = 0;
		virtual std::optional<RaycastResult> raycastFront(const glm::vec3& start, const glm::vec3& end) = 0;
		virtual void raycast(const glm::vec3& start, const glm::vec3& end, const RaycastCallback& callback) = 0;
		
		virtual void applyForce(entt::entity entity, const glm::vec3 worldForce) = 0;
		virtual void applyForceAtPoint(entt::entity entity, const glm::vec3 worldForce, const glm::vec3 worldPoint) = 0;
		virtual void applyTorque(entt::entity entity, const glm::vec3& worldTorque) = 0;

		virtual bool testOverlap(entt::entity entity1, entt::entity entity2) = 0;
		virtual void setEventCallback(const EventCallback& callback) = 0;
	};

	class IPhysicsDebugSystem {
	public:
		virtual ~IPhysicsDebugSystem() = default;

		virtual void pushDebugLine(const glm::vec3& start, const glm::vec3& end) = 0;
		virtual void update(bool enabled) = 0;
		virtual void setEnabled(bool enabled) = 0;
	};

}