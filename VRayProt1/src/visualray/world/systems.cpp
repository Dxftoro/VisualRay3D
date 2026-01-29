#include "vrpch.h"
#include "systems.h"

#include "render_service/renderer.h"
#include "event_service/event.h"

namespace vray {

	void CameraSystem::setActiveCamera(CompCamera* camera) {
		renderer->setCamera(camera);
	}

	CompCamera CameraSystem::createCamera(float fovDegrees, float far, float near) {
		Window* window = renderer->getCurrentWindow();
		return CompCamera(fovDegrees, window->getWidth(), window->getHeight(), far, near);
	}

}