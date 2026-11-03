#pragma once

#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/EnTT/entt.hpp"

#include "../spatial_service/frustum.h"
#include "render_lists.h"
#include "gpu_resource_managers.h"
#include "camera_system.h"
#include "mesh_system.h"
#include "light_system.h"

#define VR_RENDERER_MAX_DEBUG_LINES     1000

namespace vray {

	class Renderer {
	private:
		RenderLists renderLists;
		GpuResourceManagers managers;
		LightSystem lightSystem;
		MeshSystem meshSystem;
		CameraSystem& cameraSystem;

		void clear();

	public:
		Renderer(entt::registry& world, CameraSystem& cameraSystem);
		~Renderer();

		void update(const Frustum& frustum, float deltaTime);
		void submit(const MeshRequest& request);
		void submit(const BillboardRequest& request);
		void setViewport(int width, int height);
		void setClearColor(const glm::vec4& color);
		void updateDebugPrimitives(const std::vector<float>& vertexData,
			const std::vector<int>& elements);

		void unforwardMesh(GpuHandle handle);
		void unforwardTexture(GpuHandle handle);
	};

}