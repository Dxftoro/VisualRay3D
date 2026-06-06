#include "vrpch.h"
#include "alsoft_source_pool.h"

namespace vray {

	AlsoftSourcePool::AlsoftSourcePool() {
		sources = {};
		alGenSources(sources.size(), sources.data());
		for (AlSourceId i = 0; i < sources.size(); i++) freeIds.push(i);
	}

	AlsoftSourcePool::~AlsoftSourcePool() {
		alDeleteSources(sources.size(), sources.data());
	}

	void AlsoftSourcePool::set(AlSourceId id, ALuint source) {
		sources[id] = source;
	}

	void AlsoftSourcePool::release(AlSourceId id) {
		alSourceStop(sources[id]);
		alSourcei(sources[id], AL_BUFFER, 0);
		freeIds.push(id);
	}

	ALuint AlsoftSourcePool::get(AlSourceId id) const {
		return sources[id];
	}

	AlSourceId AlsoftSourcePool::acquire() {
		if (freeIds.empty()) return VR_ALSOFT_SOURCE_NULL;
		AlSourceId id = freeIds.front();
		freeIds.pop();
		return sources[id];
	}

}