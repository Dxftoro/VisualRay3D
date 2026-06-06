#pragma once
#include "audio.h"

namespace vray {

	struct ALCdevice;
	struct ALCcontext;

	class AlsoftAudio : public Audio {
	private:
		ALCdevice* device;
		ALCcontext* context;

	public:
		AlsoftAudio(entt::registry& world) : Audio(world) {}
		~AlsoftAudio() override;

		void update() override;
	};

}