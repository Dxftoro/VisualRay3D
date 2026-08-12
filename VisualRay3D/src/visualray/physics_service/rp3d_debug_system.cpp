#include "kernel.h"
#include "vrpch.h"
#include "rp3d_debug_system.h"

namespace vray {

	void Rp3dDebugSystem::pushDebugLine(const glm::vec3& start, const glm::vec3& end) {
		userVertexData.push_back(start.x);
		userVertexData.push_back(start.y);
		userVertexData.push_back(start.z);

		userVertexData.push_back(end.x);
		userVertexData.push_back(end.y);
		userVertexData.push_back(end.z);
	}

	void Rp3dDebugSystem::update(bool enabled) {
		if (enabled) return;

		pushUserVertexData();

		if (this->enabled) {
			rp3d::DebugRenderer& debugRenderer = physics->getDebugRenderer();
			debugRenderer.computeDebugRenderingPrimitives(*physics->getPhysicsWorld());

			auto debugLines = debugRenderer.getLines();
			unsigned int lineCount = debugRenderer.getNbLines();

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
		}

		renderer->updateDebugPrimitives(vertexData, elements);

		vertexData.clear();
		elements.clear();
	}

	void Rp3dDebugSystem::setEnabled(bool enabled) {
		this->enabled = enabled;
		VR_ENGINE_LOGINFO("Debug system enabled call");
		rp3d::DebugRenderer& debugRenderer = physics->getDebugRenderer();
		physics->getPhysicsWorld()->setIsDebugRenderingEnabled(enabled);
		debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, enabled);
		debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, enabled);
	}

	void Rp3dDebugSystem::pushUserVertexData() {
		for (unsigned int index = 0; index < userVertexData.size(); index++) {
			vertexData.push_back(userVertexData[index]);
			elements.push_back(vertexData.size());
		}
	}

}