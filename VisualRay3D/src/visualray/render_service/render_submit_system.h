#pragma once
#include "../thirdparty/EnTT/entt.hpp"
#include "../spatial_service/spatial_system.h"
#include "../spatial_service/frustum.h"
#include "camera_system.h"

namespace vray {

	class Renderer;

	class RenderSubmitSystem {
	private:
		entt::registry& world;
		Renderer* renderer;
		Frustum frustumCache;

		void updateFrustum(CameraSystem& cameraSystem, const glm::mat4& projectionView);

	public:
		RenderSubmitSystem(Renderer* renderer, entt::registry& world);

		void update(SpatialSystem& spatialSystem, CameraSystem& cameraSystem);
	};

}