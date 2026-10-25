#pragma once
#include "../thirdparty/EnTT/entt.hpp"

#include "../world/components.h"
#include "glsl_program.h"
#include "glsl_uniform_buffer.h"

namespace vray {

	class LightSystem {
	private:		
		entt::registry& world;
		
		using DirectionalGroup = decltype(world.group<CompDirectionalLight>(entt::get<CompTransform>));
		using PointGroup = decltype(world.group<CompPointLight>(entt::get<CompTransform>));
		using SpotGroup = decltype(world.group<CompSpotLight>(entt::get<CompTransform>));

		DirectionalGroup directionalGroup;
		PointGroup pointGroup;
		SpotGroup spotGroup;

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
	};

}