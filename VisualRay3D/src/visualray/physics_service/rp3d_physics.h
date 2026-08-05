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
	
	class Rp3dEventListener;

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
		
		virtual void applyForce(entt::entity entity, const glm::vec3 worldForce) override;
		virtual void applyForceAtPoint(entt::entity entity, const glm::vec3 worldForce, const glm::vec3 worldPoint) override;
		virtual void applyTorque(entt::entity entity, const glm::vec3& worldTorque) override;

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