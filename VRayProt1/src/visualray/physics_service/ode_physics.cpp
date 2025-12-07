#include "vrpch.h"
#include "ode_physics.h"

#include "logservice.h"

namespace vray {

	constexpr float cDeltaTime = 1.0f / 60.0f;

	OdePhysics::OdePhysics(entt::registry& _world)
		: world(_world), worldId(0), spaceId(0), collisionContactGroupId(0) {

		dynamicGroup = world.group<CompHitbox>(entt::get<CompTransform>);

		dInitODE();
		dSetMessageHandler(&handleMessages);

		worldId = dWorldCreate();
		spaceId = dHashSpaceCreate(0);
		collisionContactGroupId = dJointGroupCreate(0);

		collisionData = { worldId, collisionContactGroupId };

		dWorldSetGravity(worldId, 0.0, -9.81, 0.0);
		dWorldSetCFM(worldId, 1e-4);
		dWorldSetERP(worldId, 0.8);
		dWorldSetContactMaxCorrectingVel(worldId, 10.0);
		dWorldSetContactSurfaceLayer(worldId, 0.001);

		dWorldSetAutoDisableFlag(worldId, 1);
		dWorldSetAutoDisableTime(worldId, 2);
		dWorldSetAutoDisableSteps(worldId, 10);

		dHashSpaceSetLevels(spaceId, -2, 2);

		// !!!!!!!
		dGeomID planeId = dCreatePlane(spaceId, 0.0, 1.0, 0.0, 0.0);
	}

	OdePhysics::~OdePhysics() {
		dSpaceDestroy(spaceId);
		dWorldDestroy(worldId);
		dCloseODE();
	}

	BodyTableIterator OdePhysics::createPhysicsBody(entt::entity entity,
		const CompHitbox& hitbox, const CompTransform& transform) {

		dBodyID bodyId = nullptr;
		dGeomID geomId = 0;
		dMass bodyMass;
		dMassSetZero(&bodyMass);

		const glm::vec3& position = transform.getPosition();
		const glm::quat& rotation = transform.getRotation();
		dQuaternion bodyRotation = VR_ODEDQ(rotation);

		switch (hitbox.shapeType) {
		case CompHitbox::ShapeType::BOX: {
			glm::vec3 massVector(hitbox.mass);
			dMassSetBox(&bodyMass, VR_ODE_DEFAULT_DENSITY, VR_ODED(massVector));
			geomId = dCreateBox(spaceId, VR_ODED(hitbox.size));
			break;
		}

		case CompHitbox::ShapeType::CAPSULE: {
			glm::vec2 massVector(hitbox.mass);
			dMassSetCapsule(&bodyMass,
				VR_ODE_DEFAULT_DENSITY,
				OdeDirectionAxis::Z,
				VR_D2(massVector));

			geomId = dCreateCapsule(spaceId, hitbox.size.x * 2.0, hitbox.size.y * 2.0);
			break;
		}

		case CompHitbox::ShapeType::SPHERE: {
			dMassSetSphere(&bodyMass, VR_ODE_DEFAULT_DENSITY, hitbox.mass);
			geomId = dCreateSphere(spaceId, hitbox.radius);
			break;
		}
		default: break;
		}

		switch (hitbox.physType) {
		case CompHitbox::PhysType::STATIC:
			dGeomSetPosition(geomId, VR_ODED(position));
			dGeomSetQuaternion(geomId, bodyRotation);
			break;
		
		case CompHitbox::PhysType::DYNAMIC:
			bodyId = dBodyCreate(worldId);
			dBodySetPosition(bodyId, VR_ODED(position));
			dBodySetQuaternion(bodyId, bodyRotation);
			dBodySetMass(bodyId, &bodyMass);
			dBodySetDynamic(bodyId);
			dGeomSetBody(geomId, bodyId);
			break;
		
		case CompHitbox::PhysType::KINEMATIC:
			bodyId = dBodyCreate(worldId);
			dBodySetPosition(bodyId, VR_ODED(position));
			dBodySetQuaternion(bodyId, bodyRotation);
			dBodySetMass(bodyId, &bodyMass);
			dBodySetKinematic(bodyId);
			dGeomSetBody(geomId, bodyId);
			break;
		default: break;
		}

		BodyTableIterator it = bodyTable.emplace(entity, BodySyncData{ bodyId, geomId, false }).first;
		VR_ENGINE_LOGINFO("Hitbox created for entity " + std::to_string((uint32_t)entity));
		return it;
	}

	void OdePhysics::update(float deltaTime) {
		dynamicGroup.each([this](entt::entity entity,
			const CompHitbox& hitbox, const CompTransform& transform) {
			auto it = bodyTable.find(entity);
			if (it == bodyTable.end()) it = createPhysicsBody(entity, hitbox, transform);

			// !!!!!!!!!!!!!!!
			if (hitbox.physType == CompHitbox::PhysType::STATIC) return;

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
		dJointGroupEmpty(collisionContactGroupId);

		dynamicGroup.each([this](entt::entity entity, CompHitbox& hitbox, CompTransform& transform) {
			auto it = bodyTable.find(entity);
			BodySyncData& bodySyncData = it->second;
			
			if (hitbox.physType != CompHitbox::PhysType::STATIC) {
				const dReal* position = dBodyGetPosition(bodySyncData.bodyId);
				const dReal* rotation = dBodyGetQuaternion(bodySyncData.bodyId);
				transform.setPosition(VR_GLMD(position));
				transform.setRotation(VR_GLMDQ(rotation));
			}
			else {
				const dReal* position = dGeomGetPosition(bodySyncData.geomId);
				dReal rotation[4];
				dGeomGetQuaternion(bodySyncData.geomId, rotation);
				transform.setPosition(VR_GLMD(position));
				transform.setRotation(VR_GLMDQ(rotation));
			}

		});
	}

	void OdePhysics::handleCollisions(void* data, dGeomID geom1, dGeomID geom2) {
		auto collisionData = static_cast<CollisionData*>(data);

		dBodyID body1 = dGeomGetBody(geom1);
		dBodyID body2 = dGeomGetBody(geom2);

		if (!body1 && !body2) return;

		dContact contacts[VR_MAX_NUM_CONTACTS];

		int numc = dCollide(geom1, geom2, VR_MAX_NUM_CONTACTS, &contacts[0].geom, sizeof(dContact));

		for (int i = 0; i < numc; i++) {
			contacts[i].surface.mode = /*dContactSoftERP | dContactSoftCFM |*/ dContactApprox1 | dContactSlip1 | dContactSlip2;

			contacts[i].surface.mu = 0.8;
			//contacts[i].surface.soft_erp = 0.2;
			//contacts[i].surface.soft_cfm = 1e-5;

			dJointID contact = dJointCreateContact(collisionData->world, collisionData->contactGroup,
				&contacts[i]);

			dJointAttach(contact, body1, body2);
		}
	}

	void OdePhysics::handleMessages(int errnum, const char* msg, va_list ap) {
		/* Empty */
	}

}