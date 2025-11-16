#include "vrpch.h"
#include "systems.h"

#include "render_service/renderer.h"

namespace vray {

	void CameraSystem::setActiveCamera(CompCamera* camera) {
		renderer->setCamera(camera);
	}

}