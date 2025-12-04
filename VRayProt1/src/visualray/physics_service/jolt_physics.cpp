#include "vrpch.h"
#include "jolt_physics.h"

#include "logservice.h"

namespace vray {

	bool JoltObjectLayerPairFilter::ShouldCollide(JPH::ObjectLayer obj1, JPH::ObjectLayer obj2) const {
		switch (obj1) {
		case Layers::MOVING:
			return true;
		case Layers::NON_MOVING:
			return obj2 == Layers::MOVING;
		default:
			return false;
		}
	}

	IJoltBroadPhaseLayer::IJoltBroadPhaseLayer() {
		objectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		objectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	JPH::uint IJoltBroadPhaseLayer::GetNumBroadPhaseLayers() const {
		return BroadPhaseLayers::NUM_LAYERS;
	}

	JPH::BroadPhaseLayer IJoltBroadPhaseLayer::GetBroadPhaseLayer(JPH::ObjectLayer objLayer) const {
		return objectToBroadPhase[objLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char* IJoltBroadPhaseLayer::GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const {
		switch ((BroadPhaseLayer::Type)inLayer) {
		case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
		case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
		default:													JPH_ASSERT(false); return "INVALID";
		}
	}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	bool JoltObjectVsBroadPhaseLayerFilter::ShouldCollide(
		JPH::ObjectLayer objLayer1, JPH::BroadPhaseLayer objLayer2) const {

		switch (objLayer1) {
		case Layers::MOVING:
			return true;
		case Layers::NON_MOVING:
			return objLayer2 == BroadPhaseLayers::MOVING;
		default:
			break;
		}
	}

//	---------------------------------------------------------------------------

	JoltPhysics::JoltPhysics(entt::registry& _world)
	:	world(_world),
		tempAllocator(10 * 1024 * 1024) {

		JPH::RegisterDefaultAllocator();

		JPH::Factory::sInstance = new JPH::Factory();
		JPH::RegisterTypes();

		physicsSystem.Init(
			10,
			0,
			10,
			10,
			broadPhaseLayerInterface,
			objectVsBroadPhaseLayerFilter,
			objectLayerPairFilter);
	}

	JoltPhysics::~JoltPhysics() {
		JPH::UnregisterTypes();
		delete JPH::Factory::sInstance;
	}

	//BodyTableIterator JoltPhysics::createPhysicsBody(entt::entity entity) {
	//	CompHitbox& hitbox = dynamicGroup.get<CompHitbox>(entity);
	//	CompTransform& transform = dynamicGroup.get<CompTransform>(entity);

	//	JPH::BodyInterface& bodyInterface = physicsSystem.GetBodyInterface();
	//	JPH::ShapeSettings::ShapeResult shapeResult;

	//	switch (hitbox.shapeType) {
	//	case CompHitbox::ShapeType::BOX: {
	//		JPH::BoxShapeSettings boxSettings(glmToJolt(hitbox.size));
	//		boxSettings.SetEmbedded();
	//		shapeResult = boxSettings.Create();
	//		break;
	//	}
	//	case CompHitbox::ShapeType::SPHERE: {
	//		JPH::SphereShapeSettings sphereSettings(hitbox.radius);
	//		sphereSettings.SetEmbedded();
	//		shapeResult = sphereSettings.Create();
	//		break;
	//	}
	//	default: {
	//		assert(false);
	//		break;
	//	}
	//	}

	//	JPH::ShapeRefC shape = shapeResult.Get();
	//	JPH::EMotionType bodyMotionType = JPH::EMotionType::Static;
	//	JPH::ObjectLayer bodyLayer = Layers::NON_MOVING;

	//	switch (hitbox.physType) {
	//	case CompHitbox::PhysType::STATIC: {
	//		bodyMotionType = JPH::EMotionType::Static;
	//		bodyLayer = Layers::NON_MOVING;
	//		break;
	//	}
	//	case CompHitbox::PhysType::DYNAMIC: {
	//		bodyMotionType = JPH::EMotionType::Dynamic;
	//		bodyLayer = Layers::MOVING;
	//		break;
	//	}
	//	case CompHitbox::PhysType::KINEMATIC: {
	//		bodyMotionType = JPH::EMotionType::Kinematic;
	//		bodyLayer = Layers::MOVING;
	//		break;
	//	}
	//	default: {
	//		assert(false);
	//		break;
	//	}
	//	}

	//	JPH::BodyCreationSettings bodySettings(
	//		shape,
	//		glmToJolt(transform.getPosition()),
	//		JPH::Quat::sIdentity(),
	//		bodyMotionType,
	//		bodyLayer);

	//	JPH::BodyID bodyId = bodyInterface.CreateAndAddBody(bodySettings, JPH::EActivation::Activate);
	//	BodyTableIterator it = bodyTable.emplace(entity, BodySyncData{ bodyId, false }).first;
	//	VR_ENGINE_LOGINFO("Hitbox created for entity " + std::to_string((uint32_t)entity));
	//	return it;
	//}

	void JoltPhysics::update(float deltaTime) {

	}

	JPH::Vec3 JoltPhysics::glmToJolt(const glm::vec3& vec) {
		return { vec.x, vec.y, vec.z };
	}

	JPH::Quat JoltPhysics::glmToJolt(const glm::quat& quat) {
		return { quat.x, quat.y, quat.z, quat.w };
	}

	glm::vec3 JoltPhysics::joltToGlm(const JPH::Vec3& vec) {
		return { vec.GetX(), vec.GetY(), vec.GetZ() };
	}

	glm::quat JoltPhysics::joltToGlm(const JPH::Quat& quat) {
		return { quat.GetW(), quat.GetX(), quat.GetY(), quat.GetZ() };
	}
}