#pragma once
#include "vrpch.h"
#include <AL/al.h>

#define VR_ALSOFT_MAX_SOURCES	256
#define VR_ALSOFT_SOURCE_NULL	-1

namespace vray {

	using AlSourceId = int;

	class AlsoftSourcePool {
	private:
		std::array<ALuint, VR_ALSOFT_MAX_SOURCES> sources;
		std::queue<AlSourceId> freeIds;

	public:
		AlsoftSourcePool();
		~AlsoftSourcePool();

		void set(AlSourceId id, ALuint source);
		void release(AlSourceId id);
		ALuint get(AlSourceId id) const;
		AlSourceId acquire();

		bool isFull() const { return freeIds.empty(); }
	};

}