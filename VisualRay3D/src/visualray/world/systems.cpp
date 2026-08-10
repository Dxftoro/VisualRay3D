#include "vrpch.h"
#include "systems.h"
#include "game.h"
#include "event_service/camera_events.h"

namespace vray {

	void CameraSystem::setActiveCamera(CompCamera* camera) {
		this->camera = camera;
		vray::CameraChangedEvent event(camera);
		ctx->onEvent(event);
	}

	CompCamera CameraSystem::createCamera(float fovDegrees, float far, float near) {
		Window* window = ctx->getWindow();
		return CompCamera(fovDegrees, window->getWidth(), window->getHeight(), far, near);
	}

}