#pragma once

#include <limits>

#include "glsl_uniform_buffer.h"
#include "gpu_lights.h"

namespace vray {

	constexpr auto LIGHT_NONE				= std::numeric_limits<unsigned int>::max();
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

		unsigned int add(GpuDirectionalLight light);
		unsigned int add(GpuPointLight light);
		unsigned int add(GpuSpotLight light);

		GpuDirectionalLight getDirectionalLight(unsigned int index);
		GpuPointLight getPointLights(unsigned int index);
		GpuSpotLight getSpotLight(unsigned int index);

		void forward();
	};

}