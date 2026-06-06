#pragma once
#include <thirdparty/EnTT/entt.hpp>

namespace vray {

	class Audio {
	private:
		entt::registry& world;

	public:
		Audio(entt::registry& _world) : world(_world) {}
		virtual ~Audio() = 0;
		virtual void update() = 0;
	};

}