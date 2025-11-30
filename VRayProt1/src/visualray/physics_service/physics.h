#pragma once
#include "kernel.h"

namespace vray {

	struct DebugGeometryData {
		unsigned int lineCount = 0;
		void* data = nullptr;
	};

	class IPhysics {
	public:
		virtual void update(float deltaTime) = 0;
		//virtual DebugGeometryData getDebugGeometry() const = 0;
	};

	class IPhysicsDebugSystem {
	public:
		virtual void update(bool enabled) = 0;
	};

}