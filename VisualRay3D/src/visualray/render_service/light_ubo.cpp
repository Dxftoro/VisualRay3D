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

	unsigned int LightUbo::add(const GpuDirectionalLight& light) {
		if (layout.directionalCount >= MAX_DIRECTIONAL_LIGHTS) return LIGHT_NONE;
		layout.directionalLights[layout.directionalCount] = light;
		return layout.directionalCount++;
	}

	unsigned int LightUbo::add(const GpuPointLight& light) {
		if (layout.pointCount >= MAX_POINT_LIGHTS) return LIGHT_NONE;
		layout.pointLights[layout.pointCount] = light;
		return layout.pointCount++;
	}

	unsigned int LightUbo::add(const GpuSpotLight& light) {
		if (layout.spotCount >= MAX_DIRECTIONAL_LIGHTS) return LIGHT_NONE;
		layout.spotLights[layout.spotCount] = light;
		return layout.spotCount++;
	}

	void LightUbo::set(unsigned int index, const GpuDirectionalLight& light) {
		assert(index < layout.directionalCount && "Directional light array index out of bounds!");
		layout.directionalLights[index] = light;
	}

	void LightUbo::set(unsigned int index, const GpuPointLight& light) {
		assert(index < layout.pointCount && "Point light array index out of bounds!");
		layout.pointLights[index] = light;
	}

	void LightUbo::set(unsigned int index, const GpuSpotLight& light) {
		assert(index < layout.spotCount && "Spot light array index out of bounds!");
		layout.spotLights[index] = light;
	}

	void LightUbo::removeDirectional(unsigned int index) {
		if (!layout.directionalCount) return;
		layout.directionalCount--;
		if (layout.directionalCount) layout.directionalLights[index] = layout.directionalLights[layout.directionalCount];
	}

	void LightUbo::removePoint(unsigned int index) {
		if (!layout.pointCount) return;
		layout.pointCount--;
		if (layout.pointCount) layout.pointLights[index] = layout.pointLights[layout.pointCount];
	}

	void LightUbo::removeSpot(unsigned int index) {
		if (!layout.spotCount) return;
		layout.spotCount--;
		if (layout.spotCount) layout.spotLights[index] = layout.spotLights[layout.spotCount];
	}

	void LightUbo::clearDirectional() {
		layout.directionalCount = 0;
	}

	void LightUbo::clearPoint() {
		layout.pointCount = 0;
	}

	void LightUbo::clearSpot() {
		layout.spotCount = 0;
	}

	void LightUbo::clearAll() {
		clearDirectional();
		clearPoint();
		clearSpot();
	}

	GpuDirectionalLight LightUbo::getDirectional(unsigned int index) const {
		return layout.directionalLights[index];
	}

	GpuPointLight LightUbo::getPoint(unsigned int index) const {
		return layout.pointLights[index];
	}

	GpuSpotLight LightUbo::getSpot(unsigned int index) const {
		return layout.spotLights[index];
	}

	void LightUbo::forward() {
		setData(&layout, sizeof(layout));
	}

}