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

	bool UniformGrid::isInBox(const Aabb& aabb, const glm::vec3& point) const {
		return	point.x >= aabb.min.x && point.x <= aabb.max.x &&
				point.y >= aabb.min.y && point.y <= aabb.max.y &&
				point.z >= aabb.min.z && point.z <= aabb.max.z;
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

}