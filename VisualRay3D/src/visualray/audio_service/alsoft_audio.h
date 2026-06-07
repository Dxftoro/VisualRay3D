#pragma once
#include "audio.h"

struct ALCdevice;
struct ALCcontext;

namespace vray {

	class AlsoftAudio : public Audio {
	private:
		ALCdevice* device;
		ALCcontext* context;
		entt::entity activeListener;

		void onListenerAdded(entt::registry& world, const entt::entity);
		void updateListener(CompSoundListener& listener);

	public:
		AlsoftAudio(entt::registry& world);
		~AlsoftAudio() override;

		void update() override;
	};

}