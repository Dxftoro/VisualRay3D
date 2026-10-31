#pragma once
#include "../thirdparty/EnTT/entt.hpp"

#include "../world/components.h"
#include "glsl_program.h"
#include "light_ubo.h"

namespace vray {

	class LightSystem {
	private:		
		LightUbo lightUbo;
		entt::registry& world;
		bool lightsChanged;
		
		using DirectionalGroup = decltype(world.group<CompDirectionalLight>(entt::get<CompTransform>));
		using PointGroup = decltype(world.group<CompPointLight>(entt::get<CompTransform>));
		using SpotGroup = decltype(world.group<CompSpotLight>(entt::get<CompTransform>));

		DirectionalGroup directionalGroup;
		PointGroup pointGroup;
		SpotGroup spotGroup;

	public:
		LightSystem(entt::registry& world);

		void init(GlslProgram& program);
		
		void updateDirectional();
		void updatePoint();
		void updateSpot();
		void update();
	};

}