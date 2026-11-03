#pragma once
#include "thirdparty/EnTT/entt.hpp"
#include "spatial_service/frustum.h"

namespace vray {

	class CameraSystem;
	class SpatialSystem;
	class Renderer;

	class RenderSubmitSystem {
	private:
		CameraSystem* cameraSystem;
		SpatialSystem* spatialSystem;
		Renderer* renderer;
		entt::registry& world;

		Frustum frustumCache;

		void updateFrustum(const glm::mat4& projectionView, const glm::vec3& position, float far);

	public:
		RenderSubmitSystem(entt::registry& world, CameraSystem* cameraSystem, SpatialSystem* spatialSystem, Renderer* renderer);

		void update();
		const Frustum& getFrustumCache() const { return frustumCache; }
	};

}