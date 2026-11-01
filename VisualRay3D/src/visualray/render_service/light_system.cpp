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
		directionalGroup.each([this, &count](entt::entity entity, CompDirectionalLight& light, CompTransform& transform) {
			if (count >= MAX_DIRECTIONAL_LIGHTS) return;

			lightUbo.set(count, GpuDirectionalLight{
				.color = glm::vec4(light.color, light.intensity),
				.direction = transform.getRotation() * FORWARD_DIRECTION
			});

			count++;
		});
	}

	void LightSystem::updatePoint(const Frustum& frustum) {
		pointGroup.each([this, &frustum](entt::entity entity, CompPointLight& light, CompTransform& transform) {
			if (!intersectsFrustum(frustum, transform.getPosition(), light.range)) return;
			
			if (lightUbo.getPointCount() >= MAX_POINT_LIGHTS) return;
			
			lightUbo.add(GpuPointLight{
				.position = glm::vec4(transform.getPosition(), light.range),
				.color = glm::vec4(light.color, light.intensity)
			});
		});
	}

	void LightSystem::updateSpot(const Frustum& frustum) {
		spotGroup.each([this, &frustum](entt::entity entity, CompSpotLight& light, CompTransform& transform) {
			if (!intersectsFrustum(frustum, transform.getPosition(), light.range)) return;

			if (lightUbo.getPointCount() >= MAX_SPOT_LIGHTS) return;

			lightUbo.add(GpuSpotLight{
				.position = glm::vec4(transform.getPosition(), light.range),
				.color = glm::vec4(light.color, light.intensity),
				.direction = glm::vec4(transform.getRotation() * FORWARD_DIRECTION, light.cutOff)
			});
		});
	}

	void LightSystem::update(const Frustum& frustum) {
		updateDirectional();
		updatePoint(frustum);
		updateSpot(frustum);

		lightUbo.bind();
		lightUbo.forward();
		lightUbo.unbind();

		lightUbo.clearAll();
	}

}