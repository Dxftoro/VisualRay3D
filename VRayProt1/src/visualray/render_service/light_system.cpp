#include "vrpch.h"
#include "kernel.h"
#include "light_system.h"

namespace vray {

	LightSystem::LightSystem(GlslProgram& _program, entt::registry& _world)
	:	program(_program), world(_world) {
		
	}

	void LightSystem::onLightAdded(entt::registry& world, const entt::entity entity) {

	}

	void LightSystem::onLightUpdated(entt::registry& world, const entt::entity entity) {

	}

	void LightSystem::onLightRemoved(entt::registry& world, const entt::entity entity) {

	}

}