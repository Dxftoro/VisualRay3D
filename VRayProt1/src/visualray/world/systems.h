#pragma once
#include "components.h"
#include "kernel.h"

namespace vray {

	class Renderer;
	class Event;

	class VRAYLIB CameraSystem {
	private:
		Renderer* renderer;

	public:
		CameraSystem(Renderer* _renderer) : renderer(_renderer) {}
		void setActiveCamera(CompCamera* camera);

		CompCamera createCamera(float fovDegrees, float near, float far);
	};

}