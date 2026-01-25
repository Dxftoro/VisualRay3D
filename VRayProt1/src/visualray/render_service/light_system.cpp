#include "vrpch.h"
#include "kernel.h"
#include "light_system.h"

namespace vray {

	LightSystem::LightSystem(GlslProgram& _program, entt::registry& _world)
		: bufferedEntites({ entt::null }), program(_program), lastLightIndex(0), world(_world) {
		lightUniformBuffer = program.createUniformBuffer("LightData", nullptr, sizeof(LightBuffer));
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

	void LightSystem::handleDeleted(entt::entity entity, CompPointLightIndex& lightIndex) {
		if (!lightIndex.deleted) return;
		else if (lightIndex.index == 0) lastLightIndex--;

		/*	Removing lights that marked as deleted by moving the last buffer element
			to recently deleted element place */
		else if (lightIndex.index > 0 && lightIndex.index <= lastLightIndex - 1) {
			auto [tailLight, tailLightIndex] = world.get<CompPointLight, CompPointLightIndex>(
				bufferedEntites[lastLightIndex - 1]);

			lightUniformBuffer.setData(
				&tailLight,
				sizeof(CompPointLight),
				lightIndex.index * sizeof(CompPointLight));

			tailLightIndex.index = lightIndex.index;
			bufferedEntites[tailLightIndex.index] = bufferedEntites[lastLightIndex - 1];
			lastLightIndex--;
		}

		world.erase<CompPointLightIndex>(entity);
	}

	void LightSystem::update() {
		lightUniformBuffer.bind();

		if (lastLightIndex) {
			lightGroup.each([this](entt::entity entity, CompPointLightIndex& lightIndex) {
				handleDeleted(entity, lightIndex);
			});
		}

		lightGroup.each([this](entt::entity entity, CompPointLight& light, CompPointLightIndex& lightIndex) {
			if (!lightIndex.dirty) return;
			lightIndex.dirty = false;

			/* Adding new light if we have any free place in the buffer */
			if (lightIndex.index == VR_RENDERER_LIGHT_NEW && lastLightIndex <= VR_RENDERER_MAX_LIGHTS - 1) {
				bufferedEntites[lastLightIndex] = entity;
				lightIndex.index = lastLightIndex;
				lightUniformBuffer.setData(&light, sizeof(light), lightIndex.index * sizeof(light));
				lastLightIndex++;
			}

			/* If we need just to update current light data */
			else {
				lightUniformBuffer.setData(&light, sizeof(light), lightIndex.index * sizeof(light));
			}
		});

		lightUniformBuffer.unbind();
	}

}