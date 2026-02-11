#include "vrpch.h"
#include "kernel.h"
#include "light_system.h"
#include "logservice.h"

namespace vray {

	LightSystem::LightSystem(entt::registry& _world)
		: bufferedEntites({ entt::null }), lastLightIndex(0), world(_world) {

		lightGroup = world.group<CompPointLightIndex>(entt::get<CompPointLight>);

		world.on_construct<CompPointLight>().connect<&LightSystem::onLightAdded>();
		world.on_update<CompPointLight>().connect<&LightSystem::onLightUpdated>();
		world.on_destroy<CompPointLight>().connect<&LightSystem::onLightRemoved>(this);
	}

	void LightSystem::initBuffer(GlslProgram& program) {
		lightUniformBuffer = program.createUniformBuffer("LightData", nullptr, sizeof(LightBuffer));
	}

	void LightSystem::onLightAdded(entt::registry& world, const entt::entity entity) {
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
			VR_LOGIMPORTANT("Component delted (not buffered)!");
			return;
		}
		//else lightIndex.deleted = true;

		lightUniformBuffer.bind();

		if (lightIndex.index == 0) {
			VR_LOGIMPORTANT("handleDelted called on light index = 0!");
			lastLightIndex--;
		}

		/*	Removing lights that marked as deleted by moving the last buffer element
			to recently deleted element place */
		else if (lightIndex.index > 0 && lightIndex.index <= lastLightIndex - 1) {
			VR_LOGIMPORTANT("handleDelted called on light index in a middle!");

			auto [tailLight, tailLightIndex] = world.get<CompPointLight, CompPointLightIndex>(
				bufferedEntites[lastLightIndex - 1]);

			setLightData(tailLight, lightIndex.index);

			tailLightIndex.index = lightIndex.index;
			bufferedEntites[tailLightIndex.index] = bufferedEntites[lastLightIndex - 1];
			lastLightIndex--;
		}

		lightUniformBuffer.setData(&lastLightIndex, sizeof(lastLightIndex));
		world.erase<CompPointLightIndex>(entity);

		lightUniformBuffer.unbind();

		VR_LOGIMPORTANT("Component delted!");
	}

	void LightSystem::setLightData(CompPointLight& light, int index) {
		lightUniformBuffer.setData(&light, sizeof(light),
			offsetof(LightBuffer, lights) + index * sizeof(light));
	}

	//void LightSystem::handleDeleted(entt::entity entity, CompPointLightIndex& lightIndex) {
	//	//if (!lightIndex.deleted) return;
	//	if (lightIndex.index == 0) {
	//		VR_LOGIMPORTANT("handleDelted called on light index = 0!");
	//		lastLightIndex--;
	//	}

	//	/*	Removing lights that marked as deleted by moving the last buffer element
	//		to recently deleted element place */
	//	else if (lightIndex.index > 0 && lightIndex.index <= lastLightIndex - 1) {
	//		VR_LOGIMPORTANT("handleDelted called on light index in a middle!");

	//		auto [tailLight, tailLightIndex] = world.get<CompPointLight, CompPointLightIndex>(
	//			bufferedEntites[lastLightIndex - 1]);

	//		setLightData(tailLight, lightIndex.index);

	//		tailLightIndex.index = lightIndex.index;
	//		bufferedEntites[tailLightIndex.index] = bufferedEntites[lastLightIndex - 1];
	//		lastLightIndex--;
	//	}

	//	lightUniformBuffer.setData(&lastLightIndex, sizeof(lastLightIndex));
	//	world.erase<CompPointLightIndex>(entity);
	//	
	//	VR_LOGIMPORTANT("Component delted!");
	//}

	void LightSystem::update() {
		lightUniformBuffer.bind();

		//if (lastLightIndex) {
		//	//VR_LOGIMPORTANT("Looking for deleted lights...");
		//	for (entt::entity entity : lightGroup) {
		//		CompPointLightIndex& lightIndex = lightGroup.get<CompPointLightIndex>(entity);
		//		VR_LOGINFO("Going to call handleDelted!");
		//		handleDeleted(entity, lightIndex);
		//	}
		//}

		//VR_LOGINFO("Going to iterate over lightGroup!");
		lightGroup.each([this](entt::entity entity, CompPointLightIndex& lightIndex, CompPointLight& light) {
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