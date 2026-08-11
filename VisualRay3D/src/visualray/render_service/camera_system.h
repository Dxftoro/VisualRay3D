#pragma once
#include "../world/components.h"
#include "kernel.h"

namespace vray {

	class Game;

	class VRAYLIB CameraSystem {
	private:
		vray::Game* ctx;
		CompCamera* camera;
		glm::mat4 viewMatrix, projectionMatrix;
		bool initialCamera;

		void updateView();

	public:
		CameraSystem(vray::Game* _ctx);
		~CameraSystem();

		void init();

		void setActiveCamera(CompCamera* camera);
		void setViewMatrix(const glm::mat4& viewMatrix);
		void setProjectionMatrix(const glm::mat4& projectionMatrix);
		void setProjectionBorders(float width, float height);

		const glm::mat4& getViewMatrix();
		const glm::mat4& getProjectionMatrix() const;
		CompCamera* getActiveCamera() const { return camera; }

		CompCamera createCamera(float fovDegrees, float near, float far);
	};

}