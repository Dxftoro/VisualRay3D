#pragma once
#include "../kernel.h"
#include "../thirdparty/EnTT/entt.hpp"
#include "../thirdparty/glm/glm.hpp"

namespace vray {

	struct VRAYLIB RaycastResult {
		glm::vec3 hitPoint, hitNormal;
		entt::entity hitEntity;
		RaycastResult() = default;
	};

}