#pragma once

#include "gpu_mesh_manager.h"
#include "gpu_texture_manager.h"

namespace vray {

	struct GpuResourceManagers {
		GpuMeshManager meshes;
		GpuTextureManager textures;
	};

}