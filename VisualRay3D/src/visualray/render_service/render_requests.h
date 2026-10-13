#pragma once
#include <stdint.h>
#include "gpu_handle.h"

namespace vray {

	class CompRenderable;
	class CompTransform;
	class CompTransformMatrices;

	struct MeshRequest {
		CompRenderable* renderable;
		CompTransform* transform;
		CompTransformMatrices* matrices;
	};


	struct BillboardRequest {

	};

}