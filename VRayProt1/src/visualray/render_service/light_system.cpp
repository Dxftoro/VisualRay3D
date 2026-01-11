#include "vrpch.h"
#include "kernel.h"
#include "light_system.h"

namespace vray {

	LightSystem::LightSystem(GlslProgram& _program, entt::registry& _world)
	:	program(_program), world(_world) {
		lastLightIndex = 0;
	}

	void LightSystem::onLightAdded(entt::registry& world, const entt::entity entity) {
		world.emplace<CompPointLightIndex>(entity).index = VR_RENDERER_LIGHT_NEW;
	}

	void LightSystem::onLightUpdated(entt::registry& world, const entt::entity entity) {
		world.get<CompPointLightIndex>(entity).dirty = true;
	}

	void LightSystem::onLightRemoved(entt::registry& world, const entt::entity entity) {
		world.get<CompPointLightIndex>(entity).deleted = true;
	}

	void LightSystem::update() {
		if (!lastLightIndex) return;

		lightGroup.each([this](entt::entity entity, CompPointLight& light, CompPointLightIndex& lightIndex) {
			if (!lightIndex.dirty) return;
			lightIndex.dirty = false;

			lightUniformBuffer.bind();

			/*	Removing lights that marked as deleted by moving the last buffer element
				to recently deleted element place */
			if (lightIndex.deleted && lightIndex.index <= lastLightIndex) {
				lightUniformBuffer.setData(
					bufferedLights[lastLightIndex],
					sizeof(CompPointLight),
					lightIndex.index * sizeof(CompPointLight));

				lastLightIndex--;
				world.erase<CompPointLight, CompPointLightIndex>(entity);
			}

			/* Adding new light if we have any free place in the buffer */
			else if (lightIndex.index == VR_RENDERER_LIGHT_NEW && lastLightIndex < VR_RENDERER_MAX_LIGHTS - 1) {
				lastLightIndex++;
				bufferedLights[lastLightIndex] = &light;
				lightIndex.index = lastLightIndex;
				lightUniformBuffer.setData(&light, sizeof(light), lightIndex.index * sizeof(light));
			}

			/* If we need just to update current light data */
			else {
				lightUniformBuffer.setData(&light, sizeof(light), lightIndex.index * sizeof(light));
			}

			lightUniformBuffer.unbind();
		});
	}

}