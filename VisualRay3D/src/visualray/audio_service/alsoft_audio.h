#pragma once
#include "audio.h"

namespace vray {

	class AlsoftAudio : public Audio {
	private:
	public:
		AlsoftAudio(entt::registry& world) : Audio(world) {}
		~AlsoftAudio() override {}

		void update() override;
	};

}