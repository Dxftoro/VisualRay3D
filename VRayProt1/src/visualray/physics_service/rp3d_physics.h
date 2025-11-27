#pragma once
#include "kernel.h"
#include "physics.h"
#include "world/components.h"

#include "thirdparty/EnTT/entt.hpp"
#include <reactphysics3d/reactphysics3d.h>

namespace vray {

	struct BodySyncData {
		rp3d::RigidBody* body;
		bool synchronized;
	};

	using BodyTableIterator = std::unordered_map<entt::entity, BodySyncData>::iterator;

	class Rp3dPhysics : public IPhysics {
	private:
		rp3d::PhysicsCommon physicsCommon;
		rp3d::PhysicsWorld* physicsWorld;
		std::unordered_map<entt::entity, BodySyncData> bodyTable;
		entt::registry& world;

		using DynamicGroup = decltype(world.group<CompHitbox>(entt::get<CompTransform>));
		DynamicGroup dynamicGroup;

	private:
		//void onEntityAdded(entt::registry& world, entt::entity entity);
		BodyTableIterator createPhysicsBody(entt::entity entity);

		static rp3d::Vector3 glmToVec3(const glm::vec3& vec);
		static rp3d::Quaternion glmToQuat(const glm::quat& quat);
		static glm::vec3 vec3ToGlm(const rp3d::Vector3& vec);
		static glm::quat quatToGlm(const rp3d::Quaternion& quat);

	public:
		Rp3dPhysics(entt::registry& world);

		virtual void update(float deltaTime) override;
	};

}