#pragma once
#include "kernel.h"
#include "physics.h"
#include "world/components.h"

#include "thirdparty/EnTT/entt.hpp"

#include <Jolt/Jolt.h>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

JPH_SUPPRESS_WARNINGS

namespace vray {

	namespace Layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING	= 0;
		static constexpr JPH::ObjectLayer MOVING		= 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS	= 2;
	};

	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer	NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer	MOVING(1);
		static constexpr JPH::uint				NUM_LAYERS(2);
	};

	class JoltObjectLayerPairFilter : public JPH::ObjectLayerPairFilter {
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer obj1, JPH::ObjectLayer obj2) const override;
	};

	class IJoltBroadPhaseLayer : public JPH::BroadPhaseLayerInterface {
	private:
		JPH::BroadPhaseLayer objectToBroadPhase[Layers::NUM_LAYERS];

	public:
		IJoltBroadPhaseLayer();

		virtual JPH::uint GetNumBroadPhaseLayers() const override;
		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer objLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
	};

	class JoltObjectVsBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter {
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer objLayer1, JPH::BroadPhaseLayer objLayer2) const override;
	};

	struct BodySyncData {
		JPH::BodyID bodyId;
		bool synchronized;
	};

	using BodyTableIterator = std::unordered_map<entt::entity, BodySyncData>::iterator;

	class JoltPhysics : public IPhysics {
	private:
		JPH::TempAllocatorImpl tempAllocator;
		IJoltBroadPhaseLayer broadPhaseLayerInterface;
		JoltObjectVsBroadPhaseLayerFilter objectVsBroadPhaseLayerFilter;
		JoltObjectLayerPairFilter objectLayerPairFilter;
		JPH::PhysicsSystem physicsSystem;

		std::unordered_map<entt::entity, BodySyncData> bodyTable;
		entt::registry& world;

		using DynamicGroup = decltype(world.group<CompHitbox>(entt::get<CompTransform>));
		DynamicGroup dynamicGroup;

	private:
		//BodyTableIterator createPhysicsBody(entt::entity entity);

		static JPH::Vec3 glmToJolt(const glm::vec3& vec);
		static JPH::Quat glmToJolt(const glm::quat& quat);
		static glm::vec3 joltToGlm(const JPH::Vec3& vec);
		static glm::quat joltToGlm(const JPH::Quat& quat);

	public:
		JoltPhysics(entt::registry& world);
		~JoltPhysics();

		virtual void update(float deltaTime) override;
	};

}