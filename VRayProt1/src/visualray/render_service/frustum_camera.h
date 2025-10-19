#pragma once
#include "vrpch.h"
#include "kernel.h"
#include <glm/glm.hpp>

namespace vray {

	class FrustumCamera {
	private:
		bool active;

		/* Observing data */
		glm::mat4 viewMatrix, projectionMatrix;

		/* Camera body data */
		glm::vec3 position, rotation;

		void updateMatrices();

	public:
		FrustumCamera(float fov, float width, float height, float near, float far);
		~FrustumCamera() {}

		void setViewMatrix(const glm::mat4& viewMatrix);
		void setProjectionMatrix(const glm::mat4& projectionMatrix) { this->projectionMatrix = projectionMatrix; }
		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);

		const glm::mat4& getViewMatrix() const { return viewMatrix; }
		const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
		const glm::vec3& getPosition() const { return position; }
		const glm::vec3& getRotation() const { return rotation; }

		bool isActive() const { return active; }
	};

}