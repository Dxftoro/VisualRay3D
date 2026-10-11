#pragma once

#include "gpu_resource_manager.h"
#include "gpu_mesh.h"
#include "../managers/resource.h"

namespace vray {

	class GpuMeshManager : public GpuResourceManager<GpuMesh, Mesh> {
	public:
		GpuHandle load(Mesh* mesh) override;
		void unload(GpuHandle handle) override;
	};

}