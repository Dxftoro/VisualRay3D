#include "vrpch.h"
#include "light_ubo.h"

namespace vray {

	LightUbo::LightUbo(GlslProgram* program)
	:	GlslUniformBuffer(program, LIGHT_UBO_NAME, nullptr, sizeof(Layout)) {

		layout.directionalCount = 0;
		layout.pointCount = 0;
		layout.spotCount = 0;
		
		std::memset(layout.directionalLights, 0, sizeof(GpuDirectionalLight) * MAX_DIRECTIONAL_LIGHTS);
		std::memset(layout.pointLights, 0, sizeof(GpuPointLight) * MAX_POINT_LIGHTS);
		std::memset(layout.spotLights, 0, sizeof(GpuSpotLight) * MAX_SPOT_LIGHTS);
	}

	unsigned int LightUbo::add(GpuDirectionalLight light) {
		if (layout.directionalCount >= MAX_DIRECTIONAL_LIGHTS) return LIGHT_NONE;
		layout.directionalLights[layout.directionalCount] = light;
		return layout.directionalCount++;
	}

	unsigned int LightUbo::add(GpuPointLight light) {
		if (layout.pointCount >= MAX_POINT_LIGHTS) return LIGHT_NONE;
		layout.pointLights[layout.pointCount] = light;
		return layout.pointCount++;
	}

	unsigned int LightUbo::add(GpuSpotLight light) {
		if (layout.spotCount >= MAX_DIRECTIONAL_LIGHTS) return LIGHT_NONE;
		layout.spotLights[layout.spotCount] = light;
		return layout.spotCount++;
	}

	void LightUbo::removeDirectional(unsigned int index) {
		layout.directionalCount--;
		if (!layout.directionalCount) return;
		layout.directionalLights[index] = layout.directionalLights[layout.directionalCount];
	}

	void LightUbo::removePoint(unsigned int index) {
		layout.pointCount--;
		if (!layout.pointCount) return;
		layout.pointLights[index] = layout.pointLights[layout.pointCount];
	}

	void LightUbo::removeSpot(unsigned int index) {
		layout.spotCount--;
		if (!layout.spotCount) return;
		layout.spotLights[index] = layout.spotLights[layout.spotCount];
	}

	GpuDirectionalLight LightUbo::getDirectional(unsigned int index) {
		return layout.directionalLights[index];
	}

	GpuPointLight LightUbo::getPoint(unsigned int index) {
		return layout.pointLights[index];
	}

	GpuSpotLight LightUbo::getSpot(unsigned int index) {
		return layout.spotLights[index];
	}

	void LightUbo::forward() {
		setData(&layout, sizeof(layout));
	}

}