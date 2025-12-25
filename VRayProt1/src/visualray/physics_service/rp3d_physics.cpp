#include "vrpch.h"
#include "rp3d_physics.h"
#include "rp3d_logger.h"

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

		switch (hitbox.physType) {
		case CompHitbox::PhysType::STATIC:
			rigidBody->setType(rp3d::BodyType::STATIC);
			rigidBody->setMass(0.0f);
			break;
		case CompHitbox::PhysType::DYNAMIC: 
			rigidBody->setType(rp3d::BodyType::DYNAMIC);
			rigidBody->setMass(hitbox.mass);
			break;
		case CompHitbox::PhysType::KINEMATIC:
			rigidBody->setType(rp3d::BodyType::KINEMATIC);
			rigidBody->setMass(0.0f);
			break;
		default: break;
		}

		switch (hitbox.shapeType) {
		case CompHitbox::ShapeType::BOX: {
			rp3d::BoxShape* boxShape = physicsCommon.createBoxShape(glmToVec3(hitbox.size));
			rigidBody->addCollider(boxShape, rp3d::Transform::identity());
			break;
		}

		case CompHitbox::ShapeType::CAPSULE: {
			rp3d::CapsuleShape* capsuleShape = physicsCommon.createCapsuleShape(hitbox.size.x, hitbox.size.y);
			rigidBody->addCollider(capsuleShape, rp3d::Transform::identity());
			break;
		}

		case CompHitbox::ShapeType::SPHERE: {
			rp3d::SphereShape* sphereShape = physicsCommon.createSphereShape(hitbox.radius);
			rigidBody->addCollider(sphereShape, rp3d::Transform::identity());
			break;
		}
		default: break;
		}

		rigidBody->setIsDebugEnabled(true);
		BodyTableIterator it = bodyTable.emplace(entity, BodySyncData{ rigidBody, false }).first;
		VR_ENGINE_LOGINFO("Hitbox created for entity " + std::to_string((uint32_t)entity));
		return it;
	}

	Rp3dPhysics::Rp3dPhysics(entt::registry& _world) : world(_world) {
		//static Rp3dLogger logger;
		//physicsCommon.setLogger(&logger);
		dynamicGroup = world.group<CompHitbox>(entt::get<CompTransform>);
		physicsWorld = physicsCommon.createPhysicsWorld();
		
		physicsWorld->setIsDebugRenderingEnabled(true);

		rp3d::DebugRenderer& debugRenderer = physicsWorld->getDebugRenderer();
		//debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
		
		//debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
		
		//debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
		
		//debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
		//debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);

		//VR_ENGINE_LOGINFO("DR ADDR: " + std::to_string(&physicsWorld->getDebugRenderer()));

		VR_ENGINE_LOGINFO("World debug enabled: " +
			std::to_string(physicsWorld->getIsDebugRenderingEnabled()));
		VR_ENGINE_LOGINFO("Collision shapes debug: " +
			std::to_string(debugRenderer.getIsDebugItemDisplayed(
				rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE)));
	}

	void Rp3dPhysics::update(float deltaTime) {
		dynamicGroup.each([this](entt::entity entity, 
			CompHitbox& hitbox, CompTransform& transform) {

			auto it = bodyTable.find(entity);
			if (it == bodyTable.end()) it = createPhysicsBody(entity);

			BodySyncData& bodySyncData = it->second;
			if (!transform.isSync()) {
				bodySyncData.body->setTransform({
					glmToVec3(transform.getPosition()),
					glmToQuat(transform.getRotation())
				});
				transform.setSync(true);
			}
		});

		physicsWorld->update(deltaTime);

		dynamicGroup.each([this](entt::entity entity, CompHitbox& hitbox, CompTransform& transform){
			if (hitbox.physType == CompHitbox::PhysType::STATIC) return;

			auto it = bodyTable.find(entity);
			BodySyncData& bodySyncData = it->second;
			const rp3d::Transform & rp3dTransform = bodySyncData.body->getTransform();

			transform.setPosition(vec3ToGlm(rp3dTransform.getPosition()));
			transform.setRotation(quatToGlm(rp3dTransform.getOrientation()));
			transform.setSync(true);
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