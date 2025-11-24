#pragma once

#ifndef REACTPHYSICS3D_H
	#include <reactphysics3d/reactphysics3d.h>
#endif // !REACTPHYSICS3D_H

namespace vray {
	struct PhysicsBodyImpl {
		rp3d::RigidBody* rigidBody				= nullptr;
		rp3d::CollisionShape* collisionShape	= nullptr;
	};
}