#include "vrpch.h"
#include "util.h"

namespace vray {

	std::string sizeToString(size_t bytes) {
		return "";
	}

	float frand(const float a, const float b) {
		return a + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (b - a));
	}

}