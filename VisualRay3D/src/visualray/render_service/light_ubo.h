#pragma once

#include <limits>

#include "glsl_uniform_buffer.h"
#include "gpu_lights.h"

namespace vray {

	constexpr auto LIGHT_NONE				= std::numeric_limits<unsigned int>::max();
	constexpr auto MAX_POINT_LIGHTS			= 8;
	constexpr auto MAX_SPOT_LIGHTS			= 8;
	constexpr auto MAX_DIRECTIONAL_LIGHTS	= 2;
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

		unsigned int add(const GpuDirectionalLight& light);
		unsigned int add(const GpuPointLight& light);
		unsigned int add(const GpuSpotLight& light);

		void set(unsigned int index, const GpuDirectionalLight& light);
		void set(unsigned int index, const GpuPointLight& light);
		void set(unsigned int index, const GpuSpotLight& light);

		void removeDirectional(unsigned int index);
		void removePoint(unsigned int index);
		void removeSpot(unsigned int index);

		GpuDirectionalLight getDirectional(unsigned int index) const;
		GpuPointLight getPoint(unsigned int index) const;
		GpuSpotLight getSpot(unsigned int index) const;

		void forward();
	};

}