#include "vrpch.h"
#include "spatial_system.h"
#include "spatial_uniform_grid.h"
#include "../world/components.h"

namespace vray {

	SpatialSystem::SpatialSystem(entt::registry& _world) : backend(nullptr), world(_world) {
		backend = new UniformGrid(world, 30.0f, 1.0f);

		world.on_construct<CompTransform>().connect<&SpatialSystem::onEntityAdded>(this);
		world.on_destroy<CompTransform>().connect<&SpatialSystem::onEntityRemoved>(this);
	}

	SpatialSystem::~SpatialSystem() {
		world.on_construct<CompTransform>().disconnect<&SpatialSystem::onEntityAdded>(this);
		world.on_destroy<CompTransform>().disconnect<&SpatialSystem::onEntityRemoved>(this);

		if (backend) delete backend;
	}

	void SpatialSystem::queryFrustum(const Frustum& frustum, FunctionRef<void(entt::entity)> callback) {
		auto start = std::chrono::high_resolution_clock::now();
		backend->queryFrustum(frustum, callback);
		auto end = std::chrono::high_resolution_clock::now();
		auto micros = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();
		VR_ENGINE_LOGIMPORTANT("Time elapsed: " + STR(micros) + "s ");
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
		size_t i = 0;

		world.view<CompTransform>().each([this, &i](entt::entity entity, CompTransform& transform) {
			if (transform.isDSpatial()) {
				backend->update(entity);
				i++;
				transform.setDSpatial(false);
			}
		});

		VR_ENGINE_LOGIMPORTANT("Backend calls: " + STR(i));
	}

}