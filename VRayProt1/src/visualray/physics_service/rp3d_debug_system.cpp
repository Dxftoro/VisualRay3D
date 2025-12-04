#include "kernel.h"
#include "vrpch.h"
#include "rp3d_debug_system.h"

namespace vray {

	void Rp3dDebugSystem::update(bool enabled) {
		if (!enabled) return;

		rp3d::DebugRenderer& debugRenderer = physics->getDebugRenderer();
		debugRenderer.computeDebugRenderingPrimitives(*physics->getPhysicsWorld());
		
		auto debugLines = debugRenderer.getLines();
		unsigned int lineCount = debugRenderer.getNbLines();
		
		//VR_ENGINE_LOGINFO(std::to_string(debugRenderer.getNbLines()));
		//VR_ENGINE_LOGINFO(std::to_string(debugRenderer.getNbTriangles()));

		for (i = 0; i < lineCount; i++) {
			vertexData.push_back(debugLines[i].point1.x);
			vertexData.push_back(debugLines[i].point1.y);
			vertexData.push_back(debugLines[i].point1.z);

			vertexData.push_back(debugLines[i].point2.x);
			vertexData.push_back(debugLines[i].point2.y);
			vertexData.push_back(debugLines[i].point2.z);
			
			elements.push_back(i * 2);
			elements.push_back(i * 2 + 1);
		}

		debugRenderer.reset();
		renderer->updateDebugPrimitives(vertexData, elements);

		vertexData.clear();
		elements.clear();
	}

}