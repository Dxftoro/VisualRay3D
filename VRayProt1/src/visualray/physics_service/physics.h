#pragma once
#include "kernel.h"
#include "thirdparty/EnTT/entt.hpp"

namespace vray {

	struct PhysicsBodyImpl;
	class Physics {
	private:
		void* physicsWorld;
		std::unordered_map<entt::entity, PhysicsBodyImpl> bodyTable;
		entt::registry& world;

	public:
		Physics(entt::registry& _world);
	};

}