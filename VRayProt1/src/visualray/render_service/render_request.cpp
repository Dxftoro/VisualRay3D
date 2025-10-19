#include "vrpch.h"
#include "kernel.h"

#include "render_request.h"
#include "managers/mesh_manager.h"
#include "vertex_array.h"

namespace vray {

	void RenderRequest::complete() const {
		mesh->getVertexArray()->drawElements(static_cast<DrawMode>(drawMode));
	}

}