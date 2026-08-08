#pragma once

#include "../thirdparty/glm/glm.hpp"

namespace vray {

	struct Frustum {
		glm::vec3 position;
		float far;
		glm::vec4 planes[6];
		glm::vec3 corners[8];
	};

}