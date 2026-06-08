#pragma once
#include "vrpch.h"
#include "../util.h"
#include <AL/al.h>

#define VR_ALSOFT_MAX_SOURCES	256
#define VR_ALSOFT_SOURCE_NULL	-1

namespace vray {

	class AlSourceId : public Strong<int> {
	public:
		using Strong<int>::Strong;
	};

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

		constexpr AlSourceId size() const { return AlSourceId(sources.size()); }
		bool isFull() const { return freeIds.empty(); }
	};

}