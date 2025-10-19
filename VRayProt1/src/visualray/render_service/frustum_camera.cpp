#include "vrpch.h"
#include "kernel.h"

#include "frustum_camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace vray {

	FrustumCamera::FrustumCamera(float fov, float width, float height, float near, float far) 
	:	projectionMatrix(glm::perspectiveFov(glm::radians(fov), width, height, near, far)),
		position(0.0f, 0.0f, 5.0f), rotation(0.0f), active(false) {

		viewMatrix = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	}

	void FrustumCamera::updateMatrices() {
		glm::vec3 front;
		front.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		front.y = sin(glm::radians(rotation.y));
		front.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		front = glm::normalize(front);

		viewMatrix = glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f)); // !!!
	}

	void FrustumCamera::setViewMatrix(const glm::mat4& viewMatrix) {
		this->viewMatrix = viewMatrix;
		updateMatrices();
	}
	void FrustumCamera::setPosition(const glm::vec3& position) {
		this->position = position;
		updateMatrices();
	}
	void FrustumCamera::setRotation(const glm::vec3& rotation) { 
		this->rotation = rotation;
		updateMatrices();
	}

}