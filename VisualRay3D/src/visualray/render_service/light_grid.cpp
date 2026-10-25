#include "vrpch.h"
#include "light_grid.h"
#include "../world/components.h"

namespace vray {

	LightGrid::LightGrid(entt::registry& _world, float _cellSize) 
	:	world(_world), cellSize(_cellSize) {

	}

	void LightGrid::insert(entt::entity entity, const Volume& volume) {
		LightCell& cell = cells[volume];

		world.emplace_or_replace<CompLightGridCell>(entity, CompLightGridCell{
			.volume = volume,
			.index = cell.size() - 1
		});
	}

	void LightGrid::insert(entt::entity entity) {
		Volume volume = calculateVolume(world.get<CompPointLight>(entity).position, cellSize); // !!!
		insert(entity, volume);
	}

	void LightGrid::remove(entt::entity entity, bool finalRemoval) {
		auto& cellComp = world.get<CompLightGridCell>(entity);
		LightCell& cell = cells[cellComp.volume];

		assert(cell.size() && "LightGrid::remove: Entity references to the empty cell!");
		assert(cellComp.index < cell.size() && "LightGrid::remove: Entity index out of bounds!");
		assert(entity == cell.get(cellComp.index) && "LightGrid::remove: Given entity do not match a cells's entity!");

		if (cell.size() > 1) {
			entt::entity last = cell.get(cell.size() - 1);
			world.get<CompLightGridCell>(last).index = cellComp.index;
			cell.set(last, cellComp.index);
		}

		if (finalRemoval) world.erase<CompLightGridCell>(entity);
		cell.pop();
	}

	void LightGrid::remove(entt::entity entity) {
		remove(entity, true);
	}

	void LightGrid::update(entt::entity entity) {
		assert(world.all_of<CompLightGridCell>(entity) && "LightGrid::update: Given entity has no CompLightGridCell");

		Volume volume = calculateVolume(world.get<CompPointLight>(entity).position, cellSize);

		if (volume != world.get<CompLightGridCell>(entity).volume) {
			remove(entity, false);
			insert(entity);
		}
	}

	void LightGrid::querySphere(const glm::vec3& center, float radius, QueryCallback callback) {

	}

}