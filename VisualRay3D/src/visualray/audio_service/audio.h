#pragma once
#include "kernel.h"
#include "../world/components.h"
#include <thirdparty/EnTT/entt.hpp>

#define VR_PLAY_SOUND(WORLD, ENTITY, LOCAL) if (!WORLD.all_of<vray::CompSoundPlay>(ENTITY)) { \
WORLD.emplace<vray::CompSoundPlay>(ENTITY, vray::CompSoundPlay{ LOCAL }); \
}

namespace vray {

	class VRAYLIB Audio {
	protected:
		entt::registry& world;

	public:
		Audio(entt::registry& _world) : world(_world) {}
		virtual ~Audio() {}
		virtual void update() = 0;
		virtual void cleanup() = 0;

		void play(entt::entity soundEntity, bool local = false) { VR_PLAY_SOUND(world, soundEntity, local) }
	};

}