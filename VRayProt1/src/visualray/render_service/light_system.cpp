#include "vrpch.h"
#include "kernel.h"
#include "light_system.h"

namespace vray {

	LightSystem::LightSystem(GlslProgram& _program, entt::registry& _world)
	:	program(_program), world(_world) {
		currentLightIndex = 0;
	}

	void LightSystem::onLightAdded(entt::registry& world, const entt::entity entity) {
		world.emplace<CompPointLightIndex>(entity, CompPointLightIndex());
	}

	void LightSystem::onLightUpdated(entt::registry& world, const entt::entity entity) {
		world.get<CompPointLightIndex>(entity).dirty = true;
	}

	void LightSystem::onLightRemoved(entt::registry& world, const entt::entity entity) {
		world.remove<CompPointLightIndex>(entity);
	}

	void LightSystem::update() {
		lightGroup.each([this](entt::entity entity, CompPointLight& light, CompPointLightIndex& lightIndex) {
			if (lightIndex.dirty) {
				lightUniformBuffer.setData(&light, sizeof(light), lightIndex.lightIndex * sizeof(light));
				lightIndex.dirty = false;
			}
		});
	}

}