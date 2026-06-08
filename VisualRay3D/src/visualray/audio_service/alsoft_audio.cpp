#include "vrpch.h"
#include "alsoft_audio.h"
#include "audio_exception.h"
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

	void AlsoftAudio::updateListener(CompSoundListener& listener) {
		const glm::vec3& position = listener.getPosition();

		alListener3f(AL_POSITION,
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
		if (sources->isFull()) {
			sound.setSourceId(VR_ALSOFT_SOURCE_NULL);
			return;
		}

		sound.setSourceId(sources->acquire());
		ALuint source = sources->get(sound.getSourceId());
	}

	void AlsoftAudio::update() {
		auto& listener = world.get<CompSoundListener>(activeListener);
		if (listener.isDirty()) {
			updateListener(listener);
			listener.setDirty(false);
		}

		playingGroup.each([this](entt::entity entity, CompSoundPlay& cmdPlay, CompSound& sound) {

		});
		world.clear<CompSoundPlay>();
	}

}