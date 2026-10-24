#include "vrpch.h"
#include "volume.h"

namespace vray {

	Volume calculateVolume(const glm::vec3& point, float cellSize) {
		return {
			(int)(std::floor(point.x / cellSize)),
			(int)(std::floor(point.y / cellSize)),
			(int)(std::floor(point.z / cellSize))
		};
	}

}