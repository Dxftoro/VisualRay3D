#include "vrpch.h"
#include "spatial_system.h"
#include "spatial_uniform_grid.h"
#include "../world/components.h"

namespace vray {

	SpatialSystem::SpatialSystem(entt::registry& _world) : backend(nullptr), world(_world) {
		backend = new UniformGrid(world, 5.0f, 1.0f);

		world.on_construct<CompTransform>().connect<SpatialSystem::onEntityAdded>(this);
		world.on_destroy<CompTransform>().connect<SpatialSystem::onEntityRemoved>(this);
	}

	SpatialSystem::~SpatialSystem() {
		world.on_construct<CompTransform>().disconnect<SpatialSystem::onEntityAdded>(this);
		world.on_destroy<CompTransform>().disconnect<SpatialSystem::onEntityRemoved>(this);

		if (backend) delete backend;
	}

	void SpatialSystem::queryFrustum(const Frustum& frustum, FunctionRef<void(entt::entity)> callback) {
		backend->queryFrustum(frustum, callback);
	}

	void SpatialSystem::queryAabb(const Aabb& aabb, FunctionRef<void(entt::entity)> callback) {
		backend->queryAabb(aabb, callback);
	}

	void SpatialSystem::querySphere(const glm::vec3& position, float radius, FunctionRef<void(entt::entity)> callback) {
		backend->querySphere(position, radius, callback);
	}

	void SpatialSystem::onEntityAdded(entt::registry& world, entt::entity entity) {
		backend->insert(entity);
	}

	void SpatialSystem::onEntityRemoved(entt::registry& world, entt::entity entity) {
		backend->remove(entity);
	}

	void SpatialSystem::update() {
		world.view<CompTransform>().each([this](entt::entity entity, CompTransform& transform) {
			if (transform.isDirty()) backend->update(entity);
		});
	}

}