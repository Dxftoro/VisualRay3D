#pragma once
#include "gpu_resource_managers.h"
#include "render_lists.h"

namespace vray {

	class MeshSystem {
	private:
		GpuResourceManagers& managers;
		MeshList& meshList;

	public:
		MeshSystem(GpuResourceManagers& managers, MeshList& meshList);

		void update();
	};

}