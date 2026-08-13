#pragma once

#include "kernel.h"
#include "spatial.h"

namespace vray {

	class VRAYLIB SpatialSystem {
	private:
		ISpatial* backend;
		entt::registry& world;

		void onEntityAdded(entt::registry& world, entt::entity entity);
		void onEntityRemoved(entt::registry& world, entt::entity entity);

	public:
		SpatialSystem(entt::registry& world);
		~SpatialSystem();

		void update();

		void queryFrustum(const Frustum& frustum, FunctionRef<void(entt::entity, CompTransform&)> callback);
		void queryAabb(const Aabb& aabb, FunctionRef<void(entt::entity)> callback);
		void querySphere(const glm::vec3& position, float radius, FunctionRef<void(entt::entity)> callback);

		void destroy(entt::entity entity);
	};

}