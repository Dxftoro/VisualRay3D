#pragma once
#include "kernel.h"
#include "physics.h"
#include "world/components.h"

#include "thirdparty/EnTT/entt.hpp"
#include <reactphysics3d/reactphysics3d.h>

namespace vray {

	class Rp3dPhysics : public IPhysics {
	private:
		rp3d::PhysicsWorld* physicsWorld;
		std::unordered_map<entt::entity, rp3d::RigidBody*> bodyTable;
		entt::registry& world;

		using DynamicGroup = decltype(world.group<CompDynamic>(entt::get<CompTransform>));
		DynamicGroup dynamicGroup;

	private:
		void onEntityAddded(entt::registry& world, entt::entity entity);

	public:
		Rp3dPhysics(entt::registry& world);

		virtual void update(float deltaTime) override;
	};

}