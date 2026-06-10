#pragma once
#include "audio.h"
#include "../world/components.h"

struct ALCdevice;
struct ALCcontext;

namespace vray {
	
	class AlsoftSourcePool;

	class AlsoftAudio : public Audio {
	private:
		ALCdevice* device;
		ALCcontext* context;
		AlsoftSourcePool* sources;
		entt::entity activeListener;
		float orientation[6];

		void onListenerAdded(entt::registry& world, const entt::entity);
		void onSoundAdded(entt::registry& world, const entt::entity);
		void updateListener(CompCamera& listener);
		void updateSourcePosition(unsigned int source, const CompSound& sound);

		using SoundGroup = decltype(world.group<CompSound>());
		using PlayingGroup = decltype(world.group<CompSoundPlay>(entt::get<CompSound>));
		
		SoundGroup soundGroup;
		PlayingGroup playingGroup;

	public:
		AlsoftAudio(entt::registry& world);
		~AlsoftAudio() override;

		void update() override;
		void cleanup() override;
	};

}