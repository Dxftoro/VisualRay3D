#include "vrpch.h"
#include "rp3d_physics.h"

#include "logservice.h"

namespace vray {

	void Rp3dPhysics::onEntityAdded(entt::registry& world, entt::entity entity) {
		if (!dynamicGroup.contains(entity)) {
			VR_ENGINE_LOGWARN("Added hitbox on entity with no transform!");
			return;
		}

		CompHitbox& hitboxInfo = world.get<CompHitbox>(entity);
		CompTransform& transform = world.get<CompTransform>(entity);

		rp3d::Transform rp3dTransform{
			glmToVec3(transform.getPosition()),
			glmToQuat(transform.getRotation())
		};

		rp3d::RigidBody* rigidBody = physicsWorld->createRigidBody(rp3dTransform);

		bodyTable[entity] = rigidBody;

		VR_ENGINE_LOGINFO("Hitbox created!");
	}

	void Rp3dPhysics::createPhysicsBody(entt::entity entity) {
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

		bodyTable[entity] = rigidBody;
		VR_ENGINE_LOGINFO("Hitbox created!");
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

	Rp3dPhysics::Rp3dPhysics(entt::registry& _world) : world(_world) {
		dynamicGroup = world.group<CompHitbox>(entt::get<CompTransform>);
		world.on_construct<CompHitbox>().connect<&Rp3dPhysics::onEntityAdded>(this);
		physicsWorld = physicsCommon.createPhysicsWorld();
	}

	void Rp3dPhysics::update(float deltaTime) {
		for (entt::entity entity : dynamicGroup) {
			auto it = bodyTable.find(entity);
			if (it == bodyTable.end()) createPhysicsBody(entity);

			const CompTransform& transform = dynamicGroup.get<CompTransform>(entity);
		}
	}

}