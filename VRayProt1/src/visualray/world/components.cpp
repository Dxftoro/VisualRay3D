#include "vrpch.h"
#include "components.h"
#include <glm/gtc/matrix_transform.hpp>

namespace vray {

	const glm::mat4& CompTransform::getTransformMatrix() {
		if (dirty) {
			transformMatrix = glm::identity<glm::mat4>();
			transformMatrix = glm::translate(transformMatrix, position);
			transformMatrix *= glm::mat4_cast(rotation);
			transformMatrix = glm::scale(transformMatrix, scale);
			dirty = false;
		}
		return transformMatrix;
	}

	const glm::mat3& CompTransform::getNormalMatrix() const {
		//if (dirty) {
		//	normalMatrix = glm::mat3(glm::transpose(glm::inverse(transformMatrix)));
		//	dirty = false;
		//}
		return normalMatrix;
	}

	void CompTransform::setPosition(const glm::vec3& position) { 
		this->position = position;
		dirty = true;
		sync = false;
	}

	void CompTransform::setRotation(const glm::vec3& rotation) {
		this->rotation = glm::quat(rotation);
		dirty = true;
		sync = false;
	}

	void CompTransform::setRotation(const glm::quat& rotation) {
		this->rotation = rotation;
		dirty = true;
		sync = false;
	}

	void CompTransform::setScale(const glm::vec3& scale) { 
		this->scale = scale;
		dirty = true;
		sync = false;
	}

	void CompTransform::setNormalMatrix(const glm::mat3 normalMatrix) {
		this->normalMatrix = normalMatrix;
	}

	CompCamera::CompCamera(float fov, float width, float height, float near, float far)
		: projectionMatrix(glm::perspectiveFov(glm::radians(fov), width, height, near, far)),
		position(0.0f, 0.0f, 5.0f), rotation(0.0f), active(false) {

		viewMatrix = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void CompCamera::updateMatrices() {
		glm::vec3 front;
		front.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		front.y = sin(glm::radians(rotation.y));
		front.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		front = glm::normalize(front);

		viewMatrix = glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f)); // !!!
	}

	void CompCamera::setViewMatrix(const glm::mat4& viewMatrix) {
		this->viewMatrix = viewMatrix;
		updateMatrices();
	}
	void CompCamera::setPosition(const glm::vec3& position) {
		this->position = position;
		updateMatrices();
	}
	void CompCamera::setRotation(const glm::vec3& rotation) {
		this->rotation = rotation;
		updateMatrices();
	}

	void CompPointLight::mergeColor(const glm::vec3& color) {
		la *= color;
		ld *= color;
		ls *= color;
	}
}