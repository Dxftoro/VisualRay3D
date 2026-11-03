#pragma once
#include "thirdparty/EnTT/entt.hpp"

namespace vray {

	class DirtySystem {
	private:
		entt::registry& world;

	public:
		DirtySystem(entt::registry& _world) : world(_world) {}
		void update();
	};

}