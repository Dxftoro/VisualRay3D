#pragma once
#include "render_list.h"
#include "render_requests.h"

namespace vray {

	using MeshList = RenderList<MeshRequest>;
	using BillboardList = RenderList<BillboardRequest>;

	struct RenderLists {
		MeshList meshes;
		BillboardList billboards;
	};

}