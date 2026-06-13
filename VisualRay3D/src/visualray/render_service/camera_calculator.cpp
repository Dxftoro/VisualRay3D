#include "vrpch.h"
#include "camera_calculator.h"
#include "../world/components.h"

namespace vray {

	void CameraCalculator::updateView(CompCamera* camera) {
		const glm::vec3& position = camera->getPosition();
		glm::vec3 front;
		camera->calculateFront(front);
		viewMatrix = glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f)); // !!!
	}

	void CameraCalculator::updateProjection(CompCamera* camera) {
		projectionMatrix = glm::perspectiveFov(
			glm::radians(camera->getFov()),
			camera->getWidth(),
			camera->getHeight(),
			camera->getNear(),
			camera->getFar()
		);
	}

	void CameraCalculator::update(CompCamera* camera) {
		updateProjection(camera);
		updateView(camera);
	}

}