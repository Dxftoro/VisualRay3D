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
			int lastLightIndex;
			CompPointLight lights[VR_RENDERER_MAX_LIGHTS];
		};

		entt::entity bufferedEntites[VR_RENDERER_MAX_LIGHTS];
		int lastLightIndex;

		GlslProgram& program;
		GlslUniformBuffer lightUniformBuffer;
		
		entt::registry& world;
		using LightGroup = decltype(world.group<CompPointLightIndex>(entt::get<CompPointLight>));
		LightGroup lightGroup;

	private:
		static void onLightAdded(entt::registry& world, const entt::entity entity);
		static void onLightUpdated(entt::registry& world, const entt::entity entity);
		static void onLightRemoved(entt::registry& world, const entt::entity entity);

	public:
		LightSystem(GlslProgram& program, entt::registry& world);
		~LightSystem() {}

		void handleDeleted(entt::entity entity, CompPointLightIndex& lightIndex);
		void update();

	};

}