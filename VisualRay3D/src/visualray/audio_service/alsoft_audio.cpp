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
		orientation[0] = 0.0f;
		orientation[1] = 0.0f;
		orientation[2] = 0.0f;
		orientation[3] = 0.0f;
		orientation[4] = 1.0f;
		orientation[5] = 0.0f;

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

	void AlsoftAudio::updateListener(CompCamera& camera) {
		const glm::vec3& position = camera.getPosition();
		glm::vec3 forward;
		camera.calculateFront(forward);

		alListener3f(AL_POSITION,
			position.x,
			position.y,
			position.z);

		orientation[0] = forward.x;
		orientation[1] = forward.y;
		orientation[2] = forward.z;

		alListenerfv(AL_ORIENTATION, orientation);
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
		auto& camera  = world.get<CompCamera>(activeListener);
		updateListener(camera);

		ALfloat listenerGain;
		alGetListenerf(AL_GAIN, &listenerGain);
		VR_ENGINE_LOGIMPORTANT("Listener gain: " + std::to_string(listenerGain));
	}

	void AlsoftAudio::onSoundAdded(entt::registry& world, const entt::entity entity) {		
		auto& sound = world.get<CompSound>(entity);
		sound.setSourceId(VR_ALSOFT_SOURCE_NULL);
	}

	void AlsoftAudio::update() {
		bool playingSomething = false;
		soundGroup.each([this, &playingSomething](entt::entity entity, CompSound& sound) {
			if (sound.getSourceId() == VR_ALSOFT_SOURCE_NULL) return;
			playingSomething = true;

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

		auto& camera = world.get<CompCamera>(activeListener);
		if (playingSomething && camera.isViewDirty()) {
			VR_ENGINE_LOGIMPORTANT("Listener camera changed!");
			updateListener(camera);
		}

		playingGroup.each([this](entt::entity entity, CompSoundPlay& cmdPlay, CompSound& sound) {
			if (sources->isFull()) return;

			AlSourceId id = sources->acquire();
			ALuint source = sources->get(id);
			alSourcei(source, AL_BUFFER, sound.getSound()->getHandle());
			
			alSourcef(source, AL_MAX_DISTANCE, sound.getMaxDistance());
			alSourcef(source, AL_ROLLOFF_FACTOR, 1.0f);
			alSourcef(source, AL_REFERENCE_DISTANCE, 5.0f);

			alSourcef(source, AL_GAIN, sound.getVolume());
			alSourcef(source, AL_PITCH, sound.getPitch());

			if (!cmdPlay.ignoreSourcePosition) {
				updateSourcePosition(source, sound);
				sound.setPositionDirty(false);
			}

			sound.setSourceId(id.get());
			alSourcePlay(source);
		});
		world.clear<CompSoundPlay>();

		ALenum error = alGetError();
		if (error != AL_NO_ERROR) {
			VR_ENGINE_LOGERROR("OpenAL error: " + std::to_string(error));
		}
	}

	void AlsoftAudio::cleanup() {
		sources->cleanup();
	}
}