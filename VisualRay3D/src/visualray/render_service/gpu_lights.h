#pragma once
#include "../thirdparty/glm/glm.hpp"

namespace vray {

	struct GpuDirectionalLight {
		glm::vec4 color;					/* r, g, b, and a as intensity */
		alignas(16) glm::vec3 direction;
	};

	struct GpuPointLight {
		glm::vec4 position;					/* x, y, z and w as range */
		glm::vec4 color;					/* r, g, b, and a as intensity */
	};

	struct GpuSpotLight {
		glm::vec4 position;					/* x, y, z and w as range */
		glm::vec4 color;					/* r, g, b, and a as intensity */
		alignas(16) glm::vec3 direction;
	};

}