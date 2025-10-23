#include "vrpch.h"
#include "components.h"
#include <glm/gtc/matrix_transform.hpp>

namespace vray {

	glm::mat4& TransformComponent::getTransformMatrix() {
		if (dirty) {
			transformMatrix = glm::translate(transformMatrix, position);
			transformMatrix = glm::rotate(transformMatrix, 90.0f, rotation);
			transformMatrix = glm::scale(transformMatrix, scale);
			dirty = false;
		}
		return transformMatrix;
	}

}