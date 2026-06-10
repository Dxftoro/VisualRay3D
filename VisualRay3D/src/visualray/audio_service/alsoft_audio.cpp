#include "vrpch.h"
#include "alsoft_audio.h"
#include "audio_exception.h"
#include "alsoft_source_pool.h"
#include "logservice.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace vray {

	AlsoftAudio::AlsoftAudio(entt::registry& _world) : Audio(_world), activeListener(entt::null) {
		device = alcOpenDevice(nullptr);
		if (!device) throw AudioException("Can't open sound device!");

		context = alcCreateContext(device, nullptr);
		if (!context || !alcMakeContextCurrent(context)) {
			alcCloseDevice(device);
			throw AudioException("Can't setup audio context!");
		}

		sources = new AlsoftSourcePool();

		soundGroup = world.group<CompSound>();
		playingGroup = world.group<CompSoundPlay>(entt::get<CompSound>);
		world.on_construct<CompSoundListener>().connect<&AlsoftAudio::onListenerAdded>(this);
		world.on_construct<CompSound>().connect<&AlsoftAudio::onSoundAdded>(this);
	}

	AlsoftAudio::~AlsoftAudio() {
		delete sources;
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	void AlsoftAudio::updateListener(const CompSoundListener& listener) {
		const glm::vec3& position = listener.getPosition();
		alListener3f(AL_POSITION,
			position.x,
			position.y,
			position.z);
	}

	void AlsoftAudio::updateSourcePosition(unsigned int source, const CompSound& sound) {
		const glm::vec3& position = sound.getPosition();
		alSource3f(source, AL_POSITION,
			position.x,
			position.y,
			position.z);
	}

	void AlsoftAudio::onListenerAdded(entt::registry& world, const entt::entity entity) {
		if (activeListener != entt::null) {
			VR_ENGINE_LOGIMPORTANT("Sound listener changed, removing previous!");
			world.erase<CompSoundListener>(activeListener);
		}

		activeListener = entity;
		auto& listener = world.get<CompSoundListener>(activeListener);
		updateListener(listener);
		listener.setDirty(false);
	}

	void AlsoftAudio::onSoundAdded(entt::registry& world, const entt::entity entity) {		
		auto& sound = world.get<CompSound>(entity);
		sound.setSourceId(VR_ALSOFT_SOURCE_NULL);

		//if (sources->isFull()) {
		//	sound.setSourceId(VR_ALSOFT_SOURCE_NULL);
		//	return;
		//}

		//sound.setSourceId(sources->acquire());
		//sound.setSourceIdDirty(false);

		//ALuint source = sources->get(sound.getSourceId());
		//alSourcei(source, AL_BUFFER, sound.getSound()->getHandle());
	}

	void AlsoftAudio::update() {
		auto& listener = world.get<CompSoundListener>(activeListener);
		if (listener.isDirty()) {
			updateListener(listener);
			listener.setDirty(false);
		}

		soundGroup.each([this](entt::entity entity, CompSound& sound) {
			if (sound.getSourceId() == VR_ALSOFT_SOURCE_NULL) return;

			AlSourceId id(sound.getSourceId());
			ALuint source = sources->get(id);

			ALint state;
			alGetSourcei(source, AL_SOURCE_STATE, &state);

			if (state == AL_PLAYING && sound.isPositionDirty()) {
				updateSourcePosition(source, sound);
				sound.setPositionDirty(false);
			}
			else if (state == AL_STOPPED) {
				sound.setSourceId(VR_ALSOFT_SOURCE_NULL);
				sources->release(id);
			}
		});

		playingGroup.each([this](entt::entity entity, CompSoundPlay& cmdPlay, CompSound& sound) {
			if (sources->isFull()) return;

			AlSourceId id = sources->acquire();
			ALuint source = sources->get(id);
			alSourcei(source, AL_BUFFER, sound.getSound()->getHandle());
			alSourcei(source, AL_GAIN, sound.getVolume());

			if (!cmdPlay.ignoreSourcePosition) {
				updateSourcePosition(source, sound);
				sound.setPositionDirty(false);
			}

			sound.setSourceId(id.get());
			alSourcePlay(source);
		});
		world.clear<CompSoundPlay>();
	}

}