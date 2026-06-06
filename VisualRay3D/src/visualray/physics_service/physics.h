#pragma once
#include "vrpch.h"
#include "kernel.h"
#include "raycast_result.h"

#include <thirdparty/glm/glm.hpp>

namespace vray {

	//struct DebugGeometryData {
	//	unsigned int lineCount = 0;
	//	void* data = nullptr;
	//};

	class IPhysics {
	public:
		virtual void update(float deltaTime) = 0;
		virtual std::optional<RaycastResult> raycast(const glm::vec3& start, const glm::vec3& end) = 0;
		virtual std::optional<RaycastResult> raycast(const glm::vec3& start, const glm::vec3& dir, float range) = 0;
	};

	class IPhysicsDebugSystem {
	public:
		virtual void pushDebugLine(const glm::vec3& start, const glm::vec3& end) = 0;
		virtual void update(bool enabled) = 0;
		virtual void setEnabled(bool enabled) = 0;
	};

}