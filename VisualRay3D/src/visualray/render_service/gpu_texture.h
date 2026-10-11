#pragma once
#include "../kernel.h"

namespace vray {

	struct GpuTexture {
		unsigned int handle;
		int width;
		int height;
		int cCount;
	};

}