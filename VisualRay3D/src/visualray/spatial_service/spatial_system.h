#pragma once

#include "spatial.h"

namespace vray {

	class SpatialSystem {
	private:
		ISpatial* backend;

	public:
		SpatialSystem(entt::registry& world);
		~SpatialSystem();

		void queryFrustum(const Frustum& frustum, FunctionRef<void(entt::entity)> callback);
		void queryAabb(const Aabb& aabb, FunctionRef<void(entt::entity)> callback);
		void querySphere(const glm::vec3& position, float radius, FunctionRef<void(entt::entity)> callback);
	};

}