#pragma once
#include "kernel.h"
#include "physics.h"
#include "world/components.h"

#include "thirdparty/EnTT/entt.hpp"
#include <reactphysics3d/reactphysics3d.h>

namespace vray {

	struct CompRp3dBody {
		rp3d::RigidBody* body;
		rp3d::Collider* collider;
	};

	class SingleRaycastCallback final : public rp3d::RaycastCallback {
	private:
		std::optional<RaycastResult> lastRaycastResult;
		rp3d::decimal extremum;
		bool back;

	public:
		SingleRaycastCallback(bool _back)
		:	lastRaycastResult(std::nullopt), back(_back),
			extremum(_back ? rp3d::decimal(1.0) : rp3d::decimal(2.0)) {}

		virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& raycastInfo) override;
		std::optional<RaycastResult>& getLastResult() { return lastRaycastResult; }
	};

	class MultipleRaycastCallback final : public rp3d::RaycastCallback {
	private:
		const IPhysics::RaycastCallback& callback;

	public:
		MultipleRaycastCallback(const IPhysics::RaycastCallback& _callback) : callback(_callback) {}

		virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& raycastInfo) override;
	};

	class Rp3dEventListener final : public rp3d::EventListener {
	private:
		IPhysics::EventCallback callback;

	public:
		Rp3dEventListener(const IPhysics::EventCallback& _callback) : callback(_callback) {}
		void onContact(const rp3d::CollisionCallback::CallbackData& data) override;
		void onTrigger(const rp3d::OverlapCallback::CallbackData& data) override;
	};

	class Rp3dPhysics final : public IPhysics {
	private:
		rp3d::PhysicsCommon physicsCommon;
		rp3d::PhysicsWorld* physicsWorld;
		Rp3dEventListener* eventListener;
		entt::registry& world;

		using DynamicGroup = decltype(world.group<CompRp3dBody>(entt::get<CompHitbox, CompTransform>));
		DynamicGroup dynamicGroup;

	private:
		CompRp3dBody createPhysicsBody(entt::entity entity);
		void onEntityAdded(entt::registry& world, entt::entity entity);
		void onEntityRemoved(entt::registry& world, entt::entity entity);

	public:
		Rp3dPhysics(entt::registry& world);
		~Rp3dPhysics();

		void update(float deltaTime) override;
		
		virtual std::optional<RaycastResult> raycastBack(const glm::vec3& start, const glm::vec3& end) override;
		virtual std::optional<RaycastResult> raycastFront(const glm::vec3& start, const glm::vec3& end) override;
		virtual void raycast(const glm::vec3& start, const glm::vec3& end, const RaycastCallback& callback) override;
		
		virtual void applyForce(entt::entity, const glm::vec3 force) override;
		virtual void applyForceAtLocalPoint(entt::entity, const glm::vec3 force, const glm::vec3 point) override;
		virtual void applyTorque(entt::entity, const glm::vec3& worlTorque) override;

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