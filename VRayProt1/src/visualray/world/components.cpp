#include "vrpch.h"
#include "components.h"
#include <glm/gtc/matrix_transform.hpp>

namespace vray {

	glm::mat4& TransformComponent::getTransformMatrix() {
		if (dirty) {
			transformMatrix = glm::translate(transformMatrix, position);
			transformMatrix *= glm::mat4_cast(rotation);
			transformMatrix = glm::scale(transformMatrix, scale);
			dirty = false;
		}
		return transformMatrix;
	}

	void TransformComponent::setRotation(glm::vec3 rotation) {
		this->rotation = glm::quat(rotation);
	}

}