#include "vrpch.h"
#include "kernel.h"
#include "light_system.h"
#include "logservice.h"

namespace vray {

	LightSystem::LightSystem(entt::registry& _world)
	:	world(_world), lightsChanged(true) {
	}

	void LightSystem::init(GlslProgram& program) {
		lightUbo = LightUbo(&program);
	}

	void LightSystem::updateDirectional() {
		int count = 0;
		directionalGroup.each([this, &count](CompDirectionalLight& light, CompTransform& transform) {
			if (count >= MAX_DIRECTIONAL_LIGHTS) return;

			GpuDirectionalLight gpuLight = {
				.color = glm::vec4(light.color, light.intensity),
				.direction = transform.getRotation() * FORWARD_DIRECTION
			};

			

			count++;
		});
	}

	void LightSystem::updatePoint() {

	}

	void LightSystem::updateSpot() {

	}

	void LightSystem::update() {
		updateDirectional();
		updatePoint();
		updateSpot();

		if (lightsChanged) {
			lightUbo.bind();
			lightUbo.forward();
			lightUbo.unbind();
		}
	}

}