#pragma once
#include "../thirdparty/EnTT/entt.hpp"

#include "../world/components.h"
#include "glsl_program.h"
#include "glsl_uniform_buffer.h"

#define VR_RENDERER_MAX_LIGHTS		16
#define VR_RENDERER_LIGHT_NEW		-1

namespace vray {

	class LightSystem {
	private:
		struct LightBuffer {
			int lightCount;
			CompPointLightData lights[VR_RENDERER_MAX_LIGHTS];
		};

		entt::entity bufferedEntites[VR_RENDERER_MAX_LIGHTS];
		int lastLightIndex;

		GlslUniformBuffer lightUniformBuffer;
		
		entt::registry& world;
		using LightGroup = decltype(world.group<CompPointLightData, CompPointLightIndex>());
		LightGroup lightGroup;

	private:
		static void onLightAdded(entt::registry& world, const entt::entity entity);
		static void onLightUpdated(entt::registry& world, const entt::entity entity);
		void onLightRemoved(entt::registry& world, const entt::entity entity);

	public:
		LightSystem(entt::registry& world);
		~LightSystem() {}

		void initBuffer(GlslProgram& program);
		//void handleDeleted(entt::entity entity, CompPointLightIndex& lightIndex);
		void update();

		void setLightData(CompPointLightData& light, int index);
	};

}