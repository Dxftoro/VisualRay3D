#pragma once

#include "../thirdparty/glm/glm.hpp"

#include "render_lists.h"
#include "gpu_resource_managers.h"
#include "camera_system.h"

#define VR_RENDERER_MAX_DEBUG_LINES     1000

namespace vray {

	class Renderer {
	private:
		RenderLists renderLists;
		GpuResourceManagers managers;
		CameraSystem& cameraSystem;

	public:
		Renderer(CameraSystem& cameraSystem);
		~Renderer();

		void clear();
		void update(float deltaTime);
		void submit(const MeshRequest& request);
		void submit(const BillboardRequest& request);

		bool onWindowResize(WindowResizeEvent& evt);

		void setClearColor(const glm::vec4& color);

		void updateDebugPrimitives(const std::vector<float>& vertexData,
			const std::vector<int>& elements);
	};

}