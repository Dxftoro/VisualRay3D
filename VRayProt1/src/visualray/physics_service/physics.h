#pragma once
#include "kernel.h"

namespace vray {

	class IPhysics {
	public:
		virtual void update(float deltaTime) = 0;
	};

}