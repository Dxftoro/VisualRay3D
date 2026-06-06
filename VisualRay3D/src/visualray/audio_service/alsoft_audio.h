#pragma once
#include "audio.h"

struct ALCdevice;
struct ALCcontext;

namespace vray {

	class AlsoftAudio : public Audio {
	private:
		ALCdevice* device;
		ALCcontext* context;

	public:
		AlsoftAudio(entt::registry& world);
		~AlsoftAudio() override;

		void update() override;
	};

}