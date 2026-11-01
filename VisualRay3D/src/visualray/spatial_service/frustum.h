#pragma once

#include "../thirdparty/glm/glm.hpp"
#include "aabb.h"

namespace vray {

	struct Frustum {
		glm::vec3 position;
		float far;
		glm::vec4 planes[6];
		glm::vec3 corners[8];
	};

	Aabb calculateFrustumAabb(const glm::vec3* corners);

	/* Frustum vs AABB */
	bool intersectsFrustum(const Frustum& frustum, const Aabb& aabb);


	/* Frustum vs sphere */
	bool intersectsFrustum(const Frustum& frustum, const glm::vec3& center, float radius);

}