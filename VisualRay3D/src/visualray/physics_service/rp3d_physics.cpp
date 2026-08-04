#include "vrpch.h"
#include "rp3d_physics.h"
#include "rp3d_logger.h"
#include "logservice.h"
#include "../event_service/physics_events.h"

namespace vray {

	rp3d::decimal RaycastCallback::notifyRaycastHit(const rp3d::RaycastInfo& info) {
		lastRaycastResult = RaycastResult();
		lastRaycastResult->hitEntity = (entt::entity)(uintptr_t)info.body->getUserData();
		lastRaycastResult->hitNormal = Rp3dPhysics::vec3ToGlm(info.worldNormal);
		lastRaycastResult->hitPoint = Rp3dPhysics::vec3ToGlm(info.worldPoint);
		return info.hitFraction;
	}

	void Rp3dEventListener::onContact(const rp3d::CollisionCallback::CallbackData& data) {
		for (rp3d::uint32 i = 0; i < data.getNbContactPairs(); ++i) {
			rp3d::CollisionCallback::ContactPair contactPair = data.getContactPair(i);

			CollisionEvent event(
				(entt::entity)(uintptr_t)contactPair.getBody1()->getUserData(),
				(entt::entity)(uintptr_t)contactPair.getBody2()->getUserData(),
				(CollisionEvent::ContactType)(int)contactPair.getEventType()
			);

			callback(event);
		}
	}

	void Rp3dEventListener::onTrigger(const rp3d::OverlapCallback::CallbackData& data) {
		for (rp3d::uint32 i = 0; i < data.getNbOverlappingPairs(); ++i) {
			rp3d::OverlapCallback::OverlapPair overlapPair = data.getOverlappingPair(i);

			TriggerEvent event(
				(entt::entity)(uintptr_t)overlapPair.getBody1()->getUserData(),
				(entt::entity)(uintptr_t)overlapPair.getBody2()->getUserData(),
				(CollisionEvent::ContactType)(int)overlapPair.getEventType()
			);

			callback(event);
		}
	}

	rp3d::RigidBody* Rp3dPhysics::createPhysicsBody(entt::entity entity) {
		auto [hitbox, transform] = world.get<CompHitbox, CompTransform>(entity);

		rp3d::Transform rp3dTransform{
			glmToVec3(transform.getPosition()),
			glmToQuat(transform.getRotation())
		};

		/*
			Setting an owner entity to this rigid body for searchless body-entity access.
			We aren't allocating memory at body's user data pointer. Instead we are just
			writing entt::entity value directly to this pointer's value.

			Dirty trick, but with no need of memory management.
		*/
		rp3d::RigidBody* rigidBody = physicsWorld->createRigidBody(rp3dTransform);
		rigidBody->setUserData((void*)(uintptr_t)entity);

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
			rp3d::BoxShape* boxShape = physicsCommon.createBoxShape(glmToVec3({
				hitbox.size.x / 2,
				hitbox.size.y / 2,
				hitbox.size.z / 2
			}));
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
		return rigidBody;
	}

	void Rp3dPhysics::onEntityAdded(entt::registry& world, entt::entity entity) {
		if (world.all_of<CompTransform, CompHitbox>(entity)
			&& !world.all_of<CompRp3dBody>(entity)) {
			
			CompRp3dBody rp3dBody{ createPhysicsBody(entity) };
			world.emplace<CompRp3dBody>(entity, rp3dBody);
		}
	}

	void Rp3dPhysics::onEntityRemoved(entt::registry& world, entt::entity entity) {
		auto* rp3dBody = world.try_get<CompRp3dBody>(entity);
		if (rp3dBody) physicsWorld->destroyRigidBody(rp3dBody->body);
	}

	Rp3dPhysics::Rp3dPhysics(entt::registry& _world) : eventListener(nullptr), world(_world) {
		dynamicGroup = world.group<CompRp3dBody>(entt::get<CompTransform>);
		physicsWorld = physicsCommon.createPhysicsWorld();

		world.on_construct<CompHitbox>().connect<&Rp3dPhysics::onEntityAdded>(this);
		world.on_construct<CompTransform>().connect<&Rp3dPhysics::onEntityAdded>(this);
		//world.on_destroy<CompHitbox>().connect<&Rp3dPhysics::onEntityRemoved>(this);
		//world.on_destroy<CompTransform>().connect<&Rp3dPhysics::onEntityRemoved>(this);
		
		physicsWorld->setIsDebugRenderingEnabled(true);

		rp3d::DebugRenderer& debugRenderer = physicsWorld->getDebugRenderer();

		VR_ENGINE_LOGINFO("World debug enabled: " +
			std::to_string(physicsWorld->getIsDebugRenderingEnabled()));
		VR_ENGINE_LOGINFO("Collision shapes debug: " +
			std::to_string(debugRenderer.getIsDebugItemDisplayed(
				rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE)));
	}

	Rp3dPhysics::~Rp3dPhysics() {
		world.on_construct<CompHitbox>().disconnect<&Rp3dPhysics::onEntityAdded>(this);
		world.on_construct<CompTransform>().disconnect<&Rp3dPhysics::onEntityAdded>(this);
		//world.on_destroy<CompHitbox>().disconnect<&Rp3dPhysics::onEntityRemoved>(this);
		//world.on_destroy<CompTransform>().disconnect<&Rp3dPhysics::onEntityRemoved>(this);
	}

	void Rp3dPhysics::setEventCallback(const EventCallback& callback) {
		if (eventListener != nullptr) delete eventListener;
		eventListener = new Rp3dEventListener(callback);
		physicsWorld->setEventListener(eventListener);
	}

	void Rp3dPhysics::update(float deltaTime) {
		dynamicGroup.each([this](entt::entity entity, 
			CompRp3dBody& body, CompTransform& transform) {

			if (transform.isSync()) return;
			
			body.body->setTransform({ 
				glmToVec3(transform.getPosition()), glmToQuat(transform.getRotation()) 
			});
			
			transform.setSync(true);
		});

		physicsWorld->update(deltaTime);

		dynamicGroup.each([this](entt::entity entity, CompRp3dBody& body, CompTransform& transform){
			auto& hitbox = world.get<CompHitbox>(entity);
			if (hitbox.physType == CompHitbox::PhysType::STATIC) return;

			const rp3d::Transform& rp3dTransform = body.body->getTransform();

			transform.setPosition(vec3ToGlm(rp3dTransform.getPosition()));
			transform.setRotation(quatToGlm(rp3dTransform.getOrientation()));
			transform.setSync(true);
		});
	}

	std::optional<RaycastResult> Rp3dPhysics::raycast(const glm::vec3& start, const glm::vec3& end) {
		RaycastCallback callback;
		physicsWorld->raycast({ glmToVec3(start), glmToVec3(end) }, &callback);
		return callback.getLastResult();
	}

	std::optional<RaycastResult> Rp3dPhysics::raycast(const glm::vec3& start, const glm::vec3& dir, float range) {
		RaycastCallback callback;
		physicsWorld->raycast({ glmToVec3(start), glmToVec3(start + (dir * range)) }, &callback);
		return callback.getLastResult();
	}

	bool Rp3dPhysics::testOverlap(entt::entity entity1, entt::entity entity2) {
		auto* body1 = world.try_get<CompRp3dBody>(entity1);
		auto* body2 = world.try_get<CompRp3dBody>(entity2);

		/* TODO: Physics exception */
		if (!body1 || !body2) {
			VR_ENGINE_LOGERROR("Tested entities does not have CompTransform and CompHitbox!");
			return false;
		}

		return physicsWorld->testOverlap(body1->body, body2->body);
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