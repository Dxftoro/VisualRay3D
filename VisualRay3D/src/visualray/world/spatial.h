#pragma once

#include "aabb.h"
#include "../util/function_ref.h"
#include "../thirdparty/EnTT/entt.hpp"

namespace vray {

	enum class SpatialType {
		GRID,
		NONE
	};

	class ISpatial {
	public:
		virtual ~ISpatial() = default;

		virtual void insert(entt::entity entity) = 0;
		virtual void remove(entt::entity entity) = 0;
		virtual void queryFrustum() = 0;
	};

}