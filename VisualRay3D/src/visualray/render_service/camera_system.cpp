#include "vrpch.h"
#include "camera_system.h"
#include "game.h"
#include "event_service/camera_events.h"

namespace vray {

	CameraSystem::CameraSystem(vray::Game* _ctx)
		: ctx(_ctx), initialCamera(true), camera(nullptr),
		viewMatrix(glm::identity<glm::mat4>()),
		projectionMatrix(glm::identity<glm::mat4>()) {
	}

	CameraSystem::~CameraSystem() {
		if (initialCamera && camera != nullptr) delete camera;
	}

	void CameraSystem::init() {
		if (!initialCamera) return;
		setActiveCamera(nullptr);
	}

	void CameraSystem::setActiveCamera(CompCamera* camera) {
		if (initialCamera && this->camera != nullptr) {
			delete this->camera;
			this->camera = nullptr;
			initialCamera = false;
		}
		else if (camera == nullptr) {
			camera = new CompCamera(90, 0.1f, 300.0f);
		}

		this->camera = camera;
		Window* window = ctx->getWindow();

		projectionMatrix = glm::perspectiveFov(
			glm::radians(this->camera->getFov()),
			(float)window->getWidth(),
			(float)window->getHeight(),
			this->camera->getNear(),
			this->camera->getFar()
		);

		viewMatrix = glm::lookAt(
			this->camera->getPosition(),
			glm::vec3(0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		if (!initialCamera) {
			vray::CameraChangedEvent event(this->camera);
			ctx->onEvent(event);
		}
	}

	void CameraSystem::setViewMatrix(const glm::mat4& viewMatrix) {
		this->viewMatrix = viewMatrix;
	}

	void CameraSystem::setProjectionBorders(float width, float height) {
		projectionMatrix = glm::perspectiveFov(
			glm::radians(camera->getFov()),
			width,
			height,
			camera->getNear(),
			camera->getFar()
		);
	}

	CompCamera CameraSystem::createCamera(float fovDegrees, float far, float near) {
		Window* window = ctx->getWindow();
		return CompCamera(fovDegrees, far, near);
	}

	const glm::mat4& CameraSystem::getViewMatrix() {
		if (camera->isViewDirty()) updateView();
		return viewMatrix;
	}

	const glm::mat4& CameraSystem::getProjectionMatrix() const {
		return projectionMatrix;
	}

	void CameraSystem::updateView() {
		glm::vec3 front;
		camera->calculateFront(front);
		viewMatrix = glm::lookAt(
			camera->getPosition(),
			camera->getPosition() + front,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
	}

}