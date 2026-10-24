#include "vrpch.h"
#include "frustum.h"

namespace vray {

	Aabb calculateFrustumAabb(const glm::vec3* corners) {
		Aabb result = { corners[0], corners[0] };

		for (int i = 1; i < 8; ++i) {
			result.min = glm::min(result.min, corners[i]);
			result.max = glm::max(result.max, corners[i]);
		}

		return result;
	}

	bool intersectsFrustum(const Frustum& frustum, const Aabb& aabb) {
		for (int i = 0; i < 6; i++) {
			glm::vec3 normal(frustum.planes[i]);
			glm::vec3 positive = aabb.min;

			if (normal.x >= 0) positive.x = aabb.max.x;
			if (normal.y >= 0) positive.y = aabb.max.y;
			if (normal.z >= 0) positive.z = aabb.max.z;

			if (glm::dot(normal, positive) + frustum.planes[i].w < 0) return false;
		}

		return true;
	}

}