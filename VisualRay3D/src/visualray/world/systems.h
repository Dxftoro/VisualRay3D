#pragma once
#include "components.h"
#include "kernel.h"

namespace vray {

	class Game;

	class VRAYLIB CameraSystem {
	private:
		vray::Game* ctx;
		CompCamera* camera;
		bool cameraChanged;

	public:
		CameraSystem(vray::Game* _ctx) : ctx(_ctx), camera(nullptr) {}

		void setActiveCamera(CompCamera* camera);
		CompCamera* getActiveCamera() const { return camera; }
		CompCamera createCamera(float fovDegrees, float near, float far);
	};

}