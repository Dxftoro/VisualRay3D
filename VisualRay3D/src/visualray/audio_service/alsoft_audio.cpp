#include "vrpch.h"
#include "alsoft_audio.h"
#include "audio_exception.h"
#include "../world/components.h"
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

		world.on_construct<CompSoundListener>().connect<&AlsoftAudio::onListenerAdded>(this);
	}

	AlsoftAudio::~AlsoftAudio() {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
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

	void AlsoftAudio::update() {
		auto& listener = world.get<CompSoundListener>(activeListener);
		if (listener.isDirty()) {
			updateListener(listener);
			listener.setDirty(false);
		}


	}

}