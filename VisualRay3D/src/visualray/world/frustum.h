#pragma once

#include "../thirdparty/glm/glm.hpp"

namespace vray {

	struct Frustum {
		float near;
		float far;
		float left;
		float right;
		float top;
		float bottom;
	};

}