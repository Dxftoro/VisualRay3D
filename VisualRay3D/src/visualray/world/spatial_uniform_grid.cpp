#include "vrpch.h"
#include "spatial_uniform_grid.h"
#include "components.h"

namespace vray {

	Volume UniformGrid::calculateVolume(const glm::vec3& point) const {
		return {
			(int)(std::floor(point.x / cellSize)),
			(int)(std::floor(point.y / cellSize)),
			(int)(std::floor(point.z / cellSize))
		};
	}

	Aabb UniformGrid::calculateAabb(const glm::vec3& position, const glm::vec3& size) const {
		glm::vec3 halfSize = size * 0.5f;
		return {
			position - halfSize,
			position + halfSize
		};
	}

	Aabb UniformGrid::calculateFrustumAabb(const glm::vec3* corners) const {
		Aabb result = { corners[0], corners[0] };

		for (int i = 1; i < 8; ++i) {
			result.min = glm::min(result.min, corners[i]);
			result.max = glm::max(result.max, corners[i]);
		}

		return result;
	}

	bool UniformGrid::intersectsFrustum(const Frustum& frustum, const Aabb& aabb) const {
		for (int i = 0; i < 6; i++) {
			glm::vec3 normal(frustum.planes[i]);
			glm::vec3 positive = aabb.min;

			if (normal.x >= 0) positive.x = aabb.max.x;
			if (normal.y >= 0) positive.y = aabb.max.y;
			if (normal.z >= 0) positive.z = aabb.max.z;

			if (glm::dot(normal, positive) + frustum.planes[i].w < 0) return false;
		}

		return true;
	}

	UniformGrid::UniformGrid(entt::registry& _world, float _cellSize, float _margin)
	:	world(_world),
		cellSize(_cellSize),
		margin(_margin) {
	}

	void UniformGrid::insert(entt::entity entity, const Volume& volume) {
		std::vector<entt::entity>& list = cells[volume];
		list.push_back(entity);
		world.emplace_or_replace<CompUniformCell>(entity, volume, list.size() - 1);
	}

	void UniformGrid::insert(entt::entity entity) {
		Volume volume = calculateVolume(world.get<CompTransform>(entity).getPosition());
		insert(entity, volume);
	}

	void UniformGrid::remove(entt::entity entity, bool finalRemoval) {
		auto& cell = world.get<CompUniformCell>(entity);
		std::vector<entt::entity>& list = cells[cell.volume];

		assert(!list.empty() && "UniformGrid::remove: Entity references to the empty list!");
		assert(cell.index < list.size() && "UniformGrid::remove: Entity index out of bounds!");
		assert(entity == list[cell.index] && "UniformGrid::remove: Given entity do not match a list's entity!");

		if (list.size() > 1) {
			entt::entity last = list[list.size() - 1];
			world.get<CompUniformCell>(last).index = cell.index;
			list[cell.index] = last;
		}

		if (finalRemoval) world.erase<CompUniformCell>(entity);
		list.pop_back();
	}

	void UniformGrid::remove(entt::entity entity) { remove(entity, true); }

	void UniformGrid::update(entt::entity entity) {
		assert(world.all_of<CompUniformCell>(entity) && "UniformGrid::update: Given entity has no CompUniformCell");

		Volume volume = calculateVolume(world.get<CompTransform>(entity).getPosition());
		
		if (volume != world.get<CompUniformCell>(entity).volume) {
			remove(entity, false);
			insert(entity);
		}
	}

	void UniformGrid::queryAabb(const Aabb& aabb, FunctionRef<void(entt::entity)> callback) {
		Volume min = calculateVolume(aabb.min);
		Volume max = calculateVolume(aabb.max);

		for (int x = min.x; x <= max.x; x++) {
			for (int y = min.y; y <= max.y; y++) {
				for (int z = min.z; z <= max.z; z++) {
					Volume volume(x, y, z);
					auto it = cells.find(volume);
					if (it == cells.end()) continue;
					
					for (entt::entity entity : it->second) callback(entity);
				}
			}
		}
	}

	void UniformGrid::querySphere(const glm::vec3& center, float radius, FunctionRef<void(entt::entity)> callback) {
		Volume min = calculateVolume(center - glm::vec3(radius));
		Volume max = calculateVolume(center + glm::vec3(radius));

		for (int x = min.x; x <= max.x; x++) {
			for (int y = min.y; y <= max.y; y++) {
				for (int z = min.z; z <= max.z; z++) {
					Volume volume(x, y, z);
					auto it = cells.find(volume);
					if (it == cells.end()) continue;

					for (entt::entity entity : it->second) {
						const glm::vec3& position = world.get<CompTransform>(entity).getPosition();
						if (glm::distance(center, position) <= radius) callback(entity);
					}
				}
			}
		}
	}

	void UniformGrid::queryFrustum(const Frustum& frustum, FunctionRef<void(entt::entity)> callback) {
		Aabb bounds = calculateFrustumAabb(frustum.corners);
		Volume min = calculateVolume(bounds.min);
		Volume max = calculateVolume(bounds.max);

		for (int x = min.x; x <= max.x; x++) {
			for (int y = min.y; y <= max.y; y++) {
				for (int z = min.z; z <= max.z; z++) {
					Volume volume(x, y, z);
					auto it = cells.find(volume);
					if (it == cells.end()) continue;

					for (entt::entity entity : it->second) {
						auto& transform = world.get<CompTransform>(entity);
						const glm::vec3& position = transform.getPosition();
						const glm::vec3& size = transform.getSize();
						if (intersectsFrustum(frustum, calculateAabb(position, size))) {
							callback(entity);
						}
					}
				}
			}
		}
	}

}