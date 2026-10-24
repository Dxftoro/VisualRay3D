#include "vrpch.h"
#include "aabb.h"

namespace vray {

	Aabb calculateAabb(const glm::vec3& position, const glm::vec3& size) {
		glm::vec3 halfSize = size * 0.5f;
		return {
			position - halfSize,
			position + halfSize
		};
	}

}