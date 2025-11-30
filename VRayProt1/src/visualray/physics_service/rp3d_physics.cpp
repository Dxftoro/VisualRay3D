#include "vrpch.h"
#include "rp3d_physics.h"

#include "logservice.h"

namespace vray {

	BodyTableIterator Rp3dPhysics::createPhysicsBody(entt::entity entity) {
		CompHitbox& hitbox = dynamicGroup.get<CompHitbox>(entity);
		CompTransform& transform = dynamicGroup.get<CompTransform>(entity);

		rp3d::Transform rp3dTransform{
			glmToVec3(transform.getPosition()),
			glmToQuat(transform.getRotation())
		};

		rp3d::RigidBody* rigidBody = physicsWorld->createRigidBody(rp3dTransform);
		rigidBody->setMass(hitbox.mass);
		
		switch (hitbox.physType) {
		case CompHitbox::PhysType::STATIC:
			rigidBody->setType(rp3d::BodyType::STATIC);
			break;
		case CompHitbox::PhysType::DYNAMIC: 
			rigidBody->setType(rp3d::BodyType::DYNAMIC);
			break;
		case CompHitbox::PhysType::KINEMATIC: 
			rigidBody->setType(rp3d::BodyType::KINEMATIC);
			break;
		default: break;
		}

		switch (hitbox.shapeType) {
		case CompHitbox::ShapeType::BOX: {
			rp3d::BoxShape* boxShape = physicsCommon.createBoxShape(glmToVec3(hitbox.size));
			rigidBody->addCollider(boxShape, rp3dTransform);
			break;
		}

		case CompHitbox::ShapeType::CAPSULE: {
			rp3d::CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(hitbox.size.x, hitbox.size.y);
			rigidBody->addCollider(capsuleShape, rp3dTransform);
			break;
		}

		case CompHitbox::ShapeType::SPHERE: {
			rp3d::SphereShape* sphereShape = physicsCommon.createSphereShape(hitbox.radius);
			rigidBody->addCollider(sphereShape, rp3dTransform);
			break;
		}
		default: break;
		}

		BodyTableIterator it = bodyTable.emplace(entity, BodySyncData{ rigidBody, false }).first;
		VR_ENGINE_LOGINFO("Hitbox created for entity " + std::to_string((uint32_t)entity));
		return it;
	}

	Rp3dPhysics::Rp3dPhysics(entt::registry& _world) : world(_world) {
		dynamicGroup = world.group<CompHitbox>(entt::get<CompTransform>);
		physicsWorld = physicsCommon.createPhysicsWorld();
		
		physicsWorld->setIsDebugRenderingEnabled(true);
		physicsWorld->getDebugRenderer()
			.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	}

	void Rp3dPhysics::update(float deltaTime) {
		dynamicGroup.each([this](entt::entity entity, 
			const CompHitbox& hitbox, const CompTransform& transform) {

			auto it = bodyTable.find(entity);
			if (it == bodyTable.end()) it = createPhysicsBody(entity);

			BodySyncData& bodySyncData = it->second;
			bodySyncData.synchronized = (bodySyncData.synchronized && !transform.isDirty());

			if (!bodySyncData.synchronized) {
				bodySyncData.body->setTransform({
					glmToVec3(transform.getPosition()),
					glmToQuat(transform.getRotation())
				});
				bodySyncData.synchronized = true;
			}
		});

		physicsWorld->update(deltaTime);

		dynamicGroup.each([this](entt::entity entity, CompHitbox& hitbox, CompTransform& transform){
			auto it = bodyTable.find(entity);
			BodySyncData& bodySyncData = it->second;
			const rp3d::Transform & rp3dTransform = bodySyncData.body->getTransform();

			transform.setPosition(vec3ToGlm(rp3dTransform.getPosition()));
			transform.setRotation(quatToGlm(rp3dTransform.getOrientation()));
		});
	}

	rp3d::DebugRenderer& Rp3dPhysics::getDebugRenderer() const {
		return physicsWorld->getDebugRenderer();
	}

	rp3d::PhysicsWorld* Rp3dPhysics::getPhysicsWorld() const {
		return physicsWorld;
	}

	rp3d::Vector3 Rp3dPhysics::glmToVec3(const glm::vec3& vec) {
		return { vec.x, vec.y, vec.z };
	}

	rp3d::Quaternion Rp3dPhysics::glmToQuat(const glm::quat& quat) {
		return { quat.x, quat.y, quat.z, quat.w };
	}

	glm::vec3 Rp3dPhysics::vec3ToGlm(const rp3d::Vector3& vec) {
		return { vec.x, vec.y, vec.z };
	}

	glm::quat Rp3dPhysics::quatToGlm(const rp3d::Quaternion& quat) {
		return { quat.w, quat.x, quat.y, quat.z };
	}

}