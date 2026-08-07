#pragma once

#include "thirdparty/glm/glm.hpp"

namespace vray {

	struct Aabb {
		glm::vec3 min, max;

		Aabb expandByMargin(float margin) { return { min * margin, max * margin }; }
	};

}