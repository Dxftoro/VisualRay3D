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
		std::vector<float> userVertexData;
	
	public:
		Rp3dDebugSystem(Rp3dPhysics* _physics, Renderer* _renderer) 
			: physics(_physics), renderer(_renderer), i(0) {};

		virtual void pushDebugLine(const glm::vec3& start, const glm::vec3& end) override;
		virtual void update(bool enabled) override;
		virtual void setEnabled(bool enabled) override;

		void pushUserVertexData();
	};

}