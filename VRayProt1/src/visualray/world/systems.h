#pragma once
#include "components.h"
#include "kernel.h"

namespace vray {

	class Renderer;

	class VRAYLIB CameraSystem {
	private:
		Renderer* renderer;

	public:
		CameraSystem(Renderer* _renderer) : renderer(_renderer) {}
		void setActiveCamera(CompCamera* camera);
	};

}