#include "vrpch.h"
#include "alsoft_audio.h"
#include "audio_exception.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace vray {

	AlsoftAudio::AlsoftAudio(entt::registry& _world) : Audio(_world) {
		device = alcOpenDevice(nullptr);
		if (!device) throw AudioException("Can't open sound device!");

		context = alcCreateContext(device, nullptr);
		if (!context || !alcMakeContextCurrent(context)) {
			alcCloseDevice(device);
			throw AudioException("Can't setup audio context!");
		}
	}

}