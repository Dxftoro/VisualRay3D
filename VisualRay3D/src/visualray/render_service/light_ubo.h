#pragma once
#include "glsl_uniform_buffer.h"
#include "gpu_lights.h"

namespace vray {

	constexpr auto MAX_POINT_LIGHTS			= 8;
	constexpr auto MAX_SPOT_LIGHTS			= 8;
	constexpr auto MAX_DIRECTIONAL_LIGHTS	= 4;
	constexpr auto LIGHT_UBO_NAME			= "uLights";

	class LightUbo : public GlslUniformBuffer {
	private:
		struct Layout {
			unsigned int directionalCount;
			unsigned int pointCount;
			unsigned int spotCount;
			GpuPointLight pointLights[MAX_POINT_LIGHTS];
			GpuSpotLight spotLights[MAX_SPOT_LIGHTS];
			GpuDirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
		} layout;

	public:
		using GlslUniformBuffer::GlslUniformBuffer;
		
		LightUbo(GlslProgram* program);
	};

}