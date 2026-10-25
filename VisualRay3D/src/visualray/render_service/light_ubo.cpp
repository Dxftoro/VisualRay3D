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

}