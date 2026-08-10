#pragma once

#include "aabb.h"
#include "frustum.h"
#include "../util/function_ref.h"
#include "../thirdparty/EnTT/entt.hpp"

namespace vray {

	class ISpatial {
	public:
		virtual ~ISpatial() = default;

		virtual void insert(entt::entity entity) = 0;
		virtual void remove(entt::entity entity) = 0;
		virtual void update(entt::entity entity) = 0;
		virtual void queryFrustum(const Frustum& frustum, FunctionRef<void(entt::entity)> callback) = 0;
		virtual void queryAabb(const Aabb& aabb, FunctionRef<void(entt::entity)> callback) = 0;
		virtual void querySphere(const glm::vec3& position, float radius, FunctionRef<void(entt::entity)> callback) = 0;
	};

}