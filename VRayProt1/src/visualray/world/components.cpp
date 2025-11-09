#include "vrpch.h"
#include "components.h"
#include <glm/gtc/matrix_transform.hpp>

namespace vray {

	const glm::mat4& CompTransform::getTransformMatrix() {
		if (dirty) {
			transformMatrix = glm::translate(transformMatrix, position);
			transformMatrix *= glm::mat4_cast(rotation);
			transformMatrix = glm::scale(transformMatrix, scale);
			dirty = false;
		}
		return transformMatrix;
	}

	const glm::mat3& CompTransform::getNormalMatrix() {
		if (dirty) {
			normalMatrix = glm::mat3(glm::transpose(glm::inverse(transformMatrix)));
			dirty = false;
		}
		return normalMatrix;
	}

	void CompTransform::setPosition(glm::vec3 position) { 
		this->position = position;
		dirty = true;
	}

	void CompTransform::setRotation(glm::vec3 rotation) {
		this->rotation = glm::quat(rotation);
		dirty = true;
	}

	void CompTransform::setScale(glm::vec3 scale) { 
		this->scale = scale;
		dirty = true;
	}
}