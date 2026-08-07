#pragma once

#include "../vrpch.h"
#include "../thirdparty/glm/glm.hpp"

namespace vray {

	/* 3-dimensional vector type with integer elements */
	using Volume = glm::vec<3, int>;

	struct VolumeHash {
		size_t operator()(const Volume& volume) const {
			size_t hash = 0;

			auto merge = [&hash](int value) {
				hash ^= std::hash<int>{}(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			};

			merge(volume.x);
			merge(volume.y);
			merge(volume.z);

			return hash;
		}
	};

}
