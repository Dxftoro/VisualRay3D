#pragma once
#include "../thirdparty/EnTT/entt.hpp"

#include "../world/components.h"
#include "glsl_program.h"
#include "glsl_uniform_buffer.h"

#define VR_RENDERER_MAX_LIGHTS		10
#define VR_RENDERER_LIGHT_NOTVALID	-1

namespace vray {

	class LightSystem {
	private:
		struct LightBuffer {
			int lightCount;
			CompPointLight lights[VR_RENDERER_MAX_LIGHTS];
		};

		GlslProgram& program;
		GlslUniformBuffer lightUniformBuffer;
		int currentLightIndex;

		entt::registry& world;
		using LightGroup = decltype(world.group<CompPointLight>());
		LightGroup lightGroup;

	private:
		static void onLightAdded(entt::registry& world, const entt::entity entt);
		static void onLightRemoved(entt::registry& world, const entt::entity entt);

	public:
		LightSystem(GlslProgram& program, entt::registry& world);
		~LightSystem() {}
	};

}