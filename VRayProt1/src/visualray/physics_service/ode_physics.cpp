#include "vrpch.h"
#include "ode_physics.h"

#include "logservice.h"

namespace vray {

	OdePhysics::OdePhysics(entt::registry& _world)
		: world(_world), worldId(0), spaceId(0), collisionContactGroupId(0) {

		dynamicGroup = world.group<CompHitbox>(entt::get<CompTransform>);
		dInitODE();

		worldId = dWorldCreate();
		spaceId = dHashSpaceCreate(0);
		collisionContactGroupId = dJointGroupCreate(0);

		collisionData = { worldId, collisionContactGroupId };

		dWorldSetGravity(worldId, 0.0, 0.0, -9.81);
		dHashSpaceSetLevels(spaceId, 0, 5);

		// !!!!!!!
		dGeomID planeId = dCreatePlane(spaceId, 0.0, 0.0, 1.0, 0.0);
	}

	OdePhysics::~OdePhysics() {
		dSpaceDestroy(spaceId);
		dWorldDestroy(worldId);
		dCloseODE();
	}

	BodyTableIterator OdePhysics::createPhysicsBody(entt::entity entity,
		const CompHitbox& hitbox, const CompTransform& transform) {

		dBodyID bodyId = dBodyCreate(worldId);
		dGeomID geomId = 0;
		dMass bodyMass;

		const glm::vec3& position = transform.getPosition();
		const glm::quat& rotation = transform.getRotation();

		dQuaternion bodyRotation = VR_ODEDQ(rotation);
		dBodySetPosition(bodyId, VR_ODED(position));
		dBodySetQuaternion(bodyId, bodyRotation);

		switch (hitbox.shapeType) {
		case CompHitbox::ShapeType::BOX: {
			dMassSetBox(&bodyMass, VR_ODE_DEFAULT_DENSITY, VR_ODED(hitbox.size));

			geomId = dCreateBox(spaceId, VR_ODED(hitbox.size));
			dGeomSetBody(geomId, bodyId);
			break;
		}

		case CompHitbox::ShapeType::CAPSULE: {
			dMassSetCapsule(&bodyMass,
				VR_ODE_DEFAULT_DENSITY,
				OdeDirectionAxis::Z,
				hitbox.size.x * 2.0, hitbox.size.y * 2.0);

			geomId = dCreateCapsule(spaceId, hitbox.size.x * 2.0, hitbox.size.y * 2.0);
			dGeomSetBody(geomId, bodyId);
			break;
		}

		case CompHitbox::ShapeType::SPHERE: {
			dMassSetSphere(&bodyMass, VR_ODE_DEFAULT_DENSITY, hitbox.radius);

			geomId = dCreateSphere(spaceId, hitbox.radius);
			dGeomSetBody(geomId, bodyId);
			break;
		}
		default: break;
		}

		switch (hitbox.physType) {
		case CompHitbox::PhysType::STATIC:
			break;
		case CompHitbox::PhysType::DYNAMIC:
			dBodySetDynamic(bodyId);
			
			break;
		case CompHitbox::PhysType::KINEMATIC:
			dBodySetKinematic(bodyId);
			break;
		default: break;
		}

		dBodySetMass(bodyId, &bodyMass);

		BodyTableIterator it = bodyTable.emplace(entity, BodySyncData{ bodyId, geomId, false }).first;
		VR_ENGINE_LOGINFO("Hitbox created for entity " + std::to_string((uint32_t)entity));
		return it;
	}

	void OdePhysics::update(float deltaTime) {
		dynamicGroup.each([this](entt::entity entity,
			const CompHitbox& hitbox, const CompTransform& transform) {

			// !!!!!!!!!!!!!!!
			if (hitbox.physType == CompHitbox::PhysType::STATIC) return;

			auto it = bodyTable.find(entity);
			if (it == bodyTable.end()) it = createPhysicsBody(entity, hitbox, transform);

			BodySyncData& bodySyncData = it->second;
			bodySyncData.synchronized = (bodySyncData.synchronized && !transform.isDirty());

			if (!bodySyncData.synchronized) {
				const glm::vec3& position = transform.getPosition();
				const glm::quat& rotation = transform.getRotation();
				dQuaternion bodyRotation = VR_ODEDQ(rotation);

				dBodySetPosition(bodySyncData.bodyId, VR_ODED(position));
				dBodySetQuaternion(bodySyncData.bodyId, bodyRotation);

				bodySyncData.synchronized = true;
			}
		});

		dSpaceCollide(spaceId, &collisionData, &handleCollisions);
		dWorldStep(worldId, deltaTime);

		dynamicGroup.each([this](entt::entity entity, CompHitbox& hitbox, CompTransform& transform) {
			auto it = bodyTable.find(entity);
			BodySyncData& bodySyncData = it->second;

			const dReal* position = dBodyGetPosition(bodySyncData.bodyId);
			const dReal* rotation = dBodyGetQuaternion(bodySyncData.bodyId);

			transform.setPosition(VR_GLMD(position));
			transform.setRotation(VR_GLMDQ(rotation));
		});
	}

	void OdePhysics::handleCollisions(void* data, dGeomID geom1, dGeomID geom2) {
		auto collisionData = static_cast<CollisionData*>(data);

		dBodyID body1 = dGeomGetBody(geom1);
		dBodyID body2 = dGeomGetBody(geom2);

		dContact contacts[VR_MAX_NUM_CONTACTS];

		int numc = dCollide(geom1, geom2, VR_MAX_NUM_CONTACTS, &contacts[0].geom, sizeof(dContact));

		for (int i = 0; i < numc; i++) {
			contacts[i].surface.mode = dContactSoftERP | dContactSoftCFM | dContactApprox1
				| dContactSlip1 | dContactSlip2;

			contacts[i].surface.mu = 50.0;
			contacts[i].surface.soft_erp = 0.96;
			contacts[i].surface.soft_cfm = 2.00;

			dJointID contact = dJointCreateContact(collisionData->world, collisionData->contactGroup,
				&contacts[i]);

			dJointAttach(contact, body1, body2);
		}
	}

}