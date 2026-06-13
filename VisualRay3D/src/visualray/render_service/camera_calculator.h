#pragma once
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtc/matrix_transform.hpp>

namespace vray {

	struct CompCamera;

	class CameraCalculator {
	private:
		glm::mat4 viewMatrix, projectionMatrix;

	public:
		CameraCalculator()
			: viewMatrix(glm::identity<glm::mat4>()),
			projectionMatrix(glm::identity<glm::mat4>()) {
		}

		const glm::mat4 getViewMatrix() const { return viewMatrix; }
		const glm::mat4 getProjectionMatrix() const { return projectionMatrix; }

		void updateView(CompCamera* camera);
		void updateProjection(CompCamera* camera);
		void update(CompCamera* camera);
	};

}