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