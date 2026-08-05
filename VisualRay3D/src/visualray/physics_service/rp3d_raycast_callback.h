#pragma once
#include "../vrpch.h"
#include "physics.h"

#include <reactphysics3d/reactphysics3d.h>

namespace vray {

	class SingleRaycastCallback final : public rp3d::RaycastCallback {
	private:
		std::optional<RaycastResult> lastRaycastResult;
		rp3d::decimal extremum;
		bool back;

	public:
		SingleRaycastCallback(bool _back)
			: lastRaycastResult(std::nullopt), back(_back),
			extremum(_back ? rp3d::decimal(1.0) : rp3d::decimal(2.0)) {
		}

		virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& raycastInfo) override;
		std::optional<RaycastResult>& getLastResult() { return lastRaycastResult; }
	};

	class MultipleRaycastCallback final : public rp3d::RaycastCallback {
	private:
		const IPhysics::RaycastCallback& callback;

	public:
		MultipleRaycastCallback(const IPhysics::RaycastCallback& _callback) : callback(_callback) {}

		virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& raycastInfo) override;
	};

}