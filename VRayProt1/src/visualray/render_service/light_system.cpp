#include "vrpch.h"
#include "kernel.h"
#include "light_system.h"

namespace vray {

	LightSystem::LightSystem(GlslProgram& _program, entt::registry& _world)
	:	program(_program), world(_world) {
		
	}

}