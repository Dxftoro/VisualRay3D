#pragma once
#include "physics.h"
#include "rp3d_physics.h"
#include "render_service/renderer.h"

namespace vray {

	class Rp3dDebugSystem : public IPhysicsDebugSystem {
	private:
		Rp3dPhysics* physics;
		Renderer* renderer;

		unsigned int i;
		std::vector<float> vertexData;
		std::vector<int> elements;
	
	public:
		Rp3dDebugSystem(Rp3dPhysics* _physics, Renderer* _renderer) 
			: physics(_physics), renderer(_renderer), i(0) {};

		virtual void update(bool enabled) override;
	};

}