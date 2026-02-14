#include "vrpch.h"
#include "kernel.h"
#include "light_system.h"
#include "logservice.h"

namespace vray {

	LightSystem::LightSystem(entt::registry& _world)
		: bufferedEntites({ entt::null }), lastLightIndex(0), world(_world) {

		lightGroup = world.group<CompPointLightIndex>(entt::get<CompPointLightData>);

		world.on_construct<CompPointLight>().connect<&LightSystem::onLightAdded>();
		world.on_update<CompPointLight>().connect<&LightSystem::onLightUpdated>();
		world.on_destroy<CompPointLight>().connect<&LightSystem::onLightRemoved>(this);
	}

	void LightSystem::initBuffer(GlslProgram& program) {
		lightUniformBuffer = program.createUniformBuffer("LightData", nullptr, sizeof(LightBuffer));
	}

	void LightSystem::onLightAdded(entt::registry& world, const entt::entity entity) {
		vray::CompPointLightData light = {
			.position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			.la = glm::vec3(0.03f),
			.ld = glm::vec3(0.6),
			.ls = glm::vec3(1.0)
		};

		world.emplace<CompPointLightData>(entity, light);
		world.emplace<CompPointLightIndex>(entity).index = VR_RENDERER_LIGHT_NEW;
	}

	void LightSystem::onLightUpdated(entt::registry& world, const entt::entity entity) {
		world.get<CompPointLightIndex>(entity).dirty = true;
	}

	void LightSystem::onLightRemoved(entt::registry& world, const entt::entity entity) {
		VR_LOGIMPORTANT("onLightRemoved called!");

		CompPointLightIndex& lightIndex = world.get<CompPointLightIndex>(entity);
		VR_LOGIMPORTANT("onLightRemoved ended!");

		/* If it's not buffered */
		if (lightIndex.index == VR_RENDERER_LIGHT_NEW) {
			world.erase<CompPointLightIndex>(entity);
			world.erase<CompPointLightData>(entity);
			VR_LOGIMPORTANT("Component deleted (not buffered)!");
			return;
		}

		lightUniformBuffer.bind();

		if (lightIndex.index == 0 && lastLightIndex == 1) {
			VR_LOGIMPORTANT("handleDelted called on light index = 0!");
			lastLightIndex--;
		}

		/*	Removing lights that marked as deleted by moving the last buffer element
			to recently deleted element place */
		else if (lightIndex.index >= 0 && lightIndex.index <= lastLightIndex - 1) {
			VR_LOGIMPORTANT("handleDelted called on light index in a middle!");

			auto [tailLight, tailLightIndex] = world.get<CompPointLightData, CompPointLightIndex>(
				bufferedEntites[lastLightIndex - 1]);

			setLightData(tailLight, lightIndex.index);

			tailLightIndex.index = lightIndex.index;
			bufferedEntites[tailLightIndex.index] = bufferedEntites[lastLightIndex - 1];
			lastLightIndex--;
		}

		lightUniformBuffer.setData(&lastLightIndex, sizeof(lastLightIndex));
		world.erase<CompPointLightIndex>(entity);
		world.erase<CompPointLightData>(entity);

		lightUniformBuffer.unbind();

		VR_LOGIMPORTANT("Component deleted!");
	}

	void LightSystem::setLightData(CompPointLightData& light, int index) {
		lightUniformBuffer.setData(&light, sizeof(light),
			offsetof(LightBuffer, lights) + index * sizeof(light));
	}

	void LightSystem::update() {
		lightUniformBuffer.bind();

		lightGroup.each([this](entt::entity entity, CompPointLightIndex& lightIndex, CompPointLightData& light) {
			if (!lightIndex.dirty) return;
			lightIndex.dirty = false;

			/* Adding new light if we have any free place in the buffer */
			if (lightIndex.index == VR_RENDERER_LIGHT_NEW && lastLightIndex <= VR_RENDERER_MAX_LIGHTS - 1) {
				bufferedEntites[lastLightIndex] = entity;
				lightIndex.index = lastLightIndex;
				
				setLightData(light, lightIndex.index);
				lastLightIndex++;
				lightUniformBuffer.setData(&lastLightIndex, sizeof(lastLightIndex));
			}

			/* If we need just to update current light data */
			else {
				setLightData(light, lightIndex.index);
			}
		});

		lightUniformBuffer.unbind();
	}

}