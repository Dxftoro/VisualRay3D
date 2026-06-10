#pragma once
#include "kernel.h"
#include <thirdparty/EnTT/entt.hpp>

namespace vray {

	class VRAYLIB Audio {
	protected:
		entt::registry& world;

	public:
		Audio(entt::registry& _world) : world(_world) {}
		virtual ~Audio() {}
		virtual void update() = 0;
		virtual void cleanup() = 0;
	};

}