#include "vrpch.h"
#include "rp3d_physics.h"

#include "logservice.h"

namespace vray {

	void Rp3dPhysics::onEntityAddded(entt::registry& world, entt::entity entity) {
		VR_ENGINE_LOGINFO("Transform created!");
	}

	Rp3dPhysics::Rp3dPhysics(entt::registry& _world) : world(_world) {
		dynamicGroup = world.group<CompDynamic>(entt::get<CompTransform>);
		world.on_construct<CompTransform>().connect<&Rp3dPhysics::onEntityAddded>(this);

		rp3d::PhysicsCommon physicsCommon;
		physicsWorld = physicsCommon.createPhysicsWorld();
	}

	void Rp3dPhysics::update(float deltaTime) {
		for (entt::entity entity : dynamicGroup) {
			const CompTransform& transform = dynamicGroup.get<CompTransform>(entity);
		}
	}

}