#include "vrpch.h"
#include "alsoft_source_pool.h"
#include "logservice.h"

namespace vray {

	AlsoftSourcePool::AlsoftSourcePool() {
		sources = {};
		alGenSources(sources.size(), sources.data());
		for (AlSourceId i(0); i < sources.size(); i++) freeIds.push(i);
	}

	AlsoftSourcePool::~AlsoftSourcePool() {
		if (freeIds.size() < sources.size()) {
			VR_ENGINE_LOGWARN("Source pool has unreleased sources. Cleaning up!");
			cleanup();
		}
	}

	void AlsoftSourcePool::set(AlSourceId id, ALuint source) {
		sources[id.get()] = source;
	}

	void AlsoftSourcePool::release(AlSourceId id) {
		alSourceStop(sources[id.get()]);
		alSourcei(sources[id.get()], AL_BUFFER, 0);
		freeIds.push(id);
	}

	void AlsoftSourcePool::cleanup() {
		alSourceStopv(sources.size(), sources.data());

		while (!freeIds.empty()) freeIds.pop();

		for (AlSourceId i(0); i < sources.size(); i++) {
			alSourcei(sources[i.get()], AL_BUFFER, 0);
			freeIds.push(i);
		}

		alDeleteSources(sources.size(), sources.data());
	}

	ALuint AlsoftSourcePool::get(AlSourceId id) const {
		return sources[id.get()];
	}

	AlSourceId AlsoftSourcePool::acquire() {
		if (freeIds.empty()) return AlSourceId(VR_ALSOFT_SOURCE_NULL);
		AlSourceId id = freeIds.front();
		freeIds.pop();
		return id;
	}

}