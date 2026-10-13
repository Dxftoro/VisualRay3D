#pragma once
#include "vrpch.h"
#include "mesh_system.h"

namespace vray {

	MeshSystem::MeshSystem(GpuResourceManagers& _managers, MeshList& _meshList)
	:	managers(_managers), meshList(_meshList) {

	}

	void MeshSystem::update() {

	}

}