#include "kernel.h"
#include "vrpch.h"
#include "rp3d_debug_system.h"

namespace vray {

	BufferLayout Rp3dDebugSystem::lineDataLayout = BufferLayout({
		{ "VertexPosition", BufferObjectType::FLOAT3 }
	});

	void Rp3dDebugSystem::update(bool enabled) {
		if (!enabled) return;

		rp3d::DebugRenderer& debugRenderer = physics->getDebugRenderer();
		debugRenderer.computeDebugRenderingPrimitives(*physics->getPhysicsWorld());

		auto debugLines = debugRenderer.getLines();
		unsigned int lineCount = debugRenderer.getNbLines();

		for (i = 0; i < lineCount; i++) {
			vertexData.push_back(debugLines[i].point1.x);
			vertexData.push_back(debugLines[i].point1.y);
			vertexData.push_back(debugLines[i].point1.z);
			elements.push_back(i + 1);
		}

		debugRenderer.reset();
		renderer->drawDebugPrimitives(vertexData, elements);

		vertexData.clear();
		elements.clear();
	}

}