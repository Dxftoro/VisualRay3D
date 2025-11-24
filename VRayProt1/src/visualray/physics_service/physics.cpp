#include "vrpch.h"
#include "physics.h"

#include <reactphysics3d/reactphysics3d.h>
#include "physics_impl.h"

namespace vray {

	Physics::Physics(entt::registry& _world) : world(_world) {
		rp3d::PhysicsCommon physicsCommon;
		this->physicsWorld = (void*)physicsCommon.createPhysicsWorld();
		rp3d::PhysicsWorld* physicsWorld = (rp3d::PhysicsWorld*)this->physicsWorld;


	}

}