#pragma once
#include "physics.h"
#include <reactphysics3d/reactphysics3d.h>

namespace vray {

	class Rp3dEventListener final : public rp3d::EventListener {
	private:
		IPhysics::EventCallback callback;

	public:
		Rp3dEventListener(const IPhysics::EventCallback& _callback) : callback(_callback) {}
		void onContact(const rp3d::CollisionCallback::CallbackData& data) override;
		void onTrigger(const rp3d::OverlapCallback::CallbackData& data) override;
	};

}