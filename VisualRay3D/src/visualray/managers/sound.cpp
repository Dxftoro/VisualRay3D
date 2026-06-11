#include "vrpch.h"
#include "resource.h"
#include "logservice.h"

#include <AL/al.h>
#include <stb/stb_vorbis.h>

namespace vray {

	Sound::Sound(const std::string& filename)
		: handle(0), cCount(0), sampleRate(0), sampleCount(0) {

		short* samples;
		sampleCount = stb_vorbis_decode_filename(filename.c_str(), &cCount, &sampleRate, &samples);

		if (sampleCount == -1) {
			throw std::runtime_error("Can't load sound!");
		}

		ALenum format = (cCount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

		alGenBuffers(1, &handle);
		alBufferData(handle, format, samples, sizeof(short) * sampleCount * cCount, sampleRate);

		std::free(samples);
	}

	Sound::~Sound() {
		alDeleteBuffers(1, &handle);
	}

}