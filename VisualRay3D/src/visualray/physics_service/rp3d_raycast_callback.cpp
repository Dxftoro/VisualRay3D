#include "vrpch.h"
#include "rp3d_raycast_callback.h"
#include "rp3d_physics.h"

namespace vray {

	rp3d::decimal SingleRaycastCallback::notifyRaycastHit(const rp3d::RaycastInfo& info) {
		bool isBetter = back ? (info.hitFraction > extremum) : (info.hitFraction < extremum);

		if (isBetter) {
			extremum = info.hitFraction;
			lastRaycastResult = RaycastResult();
			lastRaycastResult->hitEntity = (entt::entity)(uintptr_t)info.body->getUserData();
			lastRaycastResult->hitNormal = Rp3dPhysics::vec3ToGlm(info.worldNormal);
			lastRaycastResult->hitPoint = Rp3dPhysics::vec3ToGlm(info.worldPoint);
		}

		return (back ? rp3d::decimal(-1.0) : info.hitFraction);
	}

	rp3d::decimal MultipleRaycastCallback::notifyRaycastHit(const rp3d::RaycastInfo& info) {
		RaycastResult result;

		result.hitEntity = (entt::entity)(uintptr_t)info.body->getUserData();
		result.hitNormal = Rp3dPhysics::vec3ToGlm(info.worldNormal);
		result.hitPoint = Rp3dPhysics::vec3ToGlm(info.worldPoint);

		callback(result);

		return 1.0;
	}

}