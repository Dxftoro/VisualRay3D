#include "vrpch.h"
#include "alsoft_audio.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace vray {

	AlsoftAudio::AlsoftAudio(entt::registry& _world) : Audio(_world) {
		device = alcOpenDevice(nullptr);

	}

}