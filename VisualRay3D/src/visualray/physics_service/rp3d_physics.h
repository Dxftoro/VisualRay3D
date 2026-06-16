#pragma once
#include "kernel.h"
#include "physics.h"
#include "world/components.h"

#include "thirdparty/EnTT/entt.hpp"
#include <reactphysics3d/reactphysics3d.h>

namespace vray {

	struct BodySyncData {
		rp3d::RigidBody* body;
		entt::entity entity;
	};

	using BodyTableIterator = std::unordered_map<entt::entity, BodySyncData>::iterator;

	class RaycastCallback : public rp3d::RaycastCallback {
	private:
		std::optional<RaycastResult> lastRaycastResult;

	public:
		RaycastCallback() : lastRaycastResult(std::nullopt) {}

		virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& raycastInfo) override;
		std::optional<RaycastResult>& getLastResult() { return lastRaycastResult; }
	};

	class Rp3dEventListener : public rp3d::EventListener {
	private:
		IPhysics::EventCallback callback;

	public:
		Rp3dEventListener(const IPhysics::EventCallback& _callback) : callback(_callback) {}
		void onContact(const rp3d::CollisionCallback::CallbackData& data) override;
		void onTrigger(const rp3d::OverlapCallback::CallbackData& data) override;
	};

	class Rp3dPhysics : public IPhysics {
	private:
		rp3d::PhysicsCommon physicsCommon;
		rp3d::PhysicsWorld* physicsWorld;
		std::unordered_map<entt::entity, BodySyncData> bodyTable;
		Rp3dEventListener* eventListener;
		entt::registry& world;

		using DynamicGroup = decltype(world.group<CompHitbox>(entt::get<CompTransform>));
		DynamicGroup dynamicGroup;

	private:
		//void onEntityAdded(entt::registry& world, entt::entity entity);
		BodyTableIterator createPhysicsBody(entt::entity entity);

	public:
		Rp3dPhysics(entt::registry& world);

		void update(float deltaTime) override;
		
		std::optional<RaycastResult> raycast(const glm::vec3& start, const glm::vec3& end) override;
		std::optional<RaycastResult> raycast(const glm::vec3& start, const glm::vec3& dir, float range) override;
		
		bool testOverlap(entt::entity entity1, entt::entity entity2) override;

		void setEventCallback(const EventCallback& callback) override;

		rp3d::DebugRenderer& getDebugRenderer() const;
		rp3d::PhysicsWorld* getPhysicsWorld() const;

		static rp3d::Vector3 glmToVec3(const glm::vec3& vec);
		static rp3d::Quaternion glmToQuat(const glm::quat& quat);
		static glm::vec3 vec3ToGlm(const rp3d::Vector3& vec);
		static glm::quat quatToGlm(const rp3d::Quaternion& quat);
	};

}