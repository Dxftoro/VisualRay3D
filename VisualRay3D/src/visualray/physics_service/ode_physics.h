#pragma once

/* ============== DEPRECATED ============== */

/*
#ifdef VR_PLATFORM_WIN_DEBUG
#include "physics.h"
#include "world/components.h"

#include "thirdparty/EnTT/entt.hpp"
#include <ode/ode.h>

#define VR_ODED(GLM_VEC)	(GLM_VEC).x, (GLM_VEC).y, (GLM_VEC).z
#define VR_ODEDQ(GLM_QUAT)	{ (GLM_QUAT).w, (GLM_QUAT).x, (GLM_QUAT).y, (GLM_QUAT).z }
#define VR_GLMD(ODE_VEC)	{ (ODE_VEC)[0], (ODE_VEC)[1], (ODE_VEC)[2] }
#define VR_GLMDQ(ODE_QUAT)	{ (ODE_QUAT)[0], (ODE_QUAT)[1], (ODE_QUAT)[2], (ODE_QUAT)[3] }

#define VR_ODE_DEFAULT_DENSITY	1.0
#define VR_MAX_NUM_CONTACTS		4

namespace vray {

	struct BodySyncData {
		dBodyID bodyId;
		dGeomID geomId;
		bool synchronized;
	};

	struct CollisionData {
		dWorldID world;
		dJointGroupID contactGroup;
	};

	namespace OdeDirectionAxis {
		constexpr int X = 1;
		constexpr int Y = 2;
		constexpr int Z = 3;
	}

	using BodyTableIterator = std::unordered_map<entt::entity, BodySyncData>::iterator;

	class OdePhysics : public IPhysics {
	private:
		dWorldID worldId;
		dSpaceID spaceId;
		dJointGroupID collisionContactGroupId;
		CollisionData collisionData;

		std::unordered_map<entt::entity, BodySyncData> bodyTable;
		entt::registry& world;

		using DynamicGroup = decltype(world.group<CompHitbox>(entt::get<CompTransform>));
		DynamicGroup dynamicGroup;

	private:
		static void handleCollisions(void* data, dGeomID geom1, dGeomID geom2);
		static void handleMessages(int errnum, const char* msg, va_list ap);

		BodyTableIterator createPhysicsBody(entt::entity entity,
			const CompHitbox& hitbox, const CompTransform& transform);

	public:
		OdePhysics(entt::registry& world);
		~OdePhysics();

		virtual void update(float deltaTime) override;
	};

}
#endif
*/