#pragma once

#include "light_cell.h"
#include "../thirdparty/EnTT/entt.hpp"
#include "../spatial_service/volume.h"
#include "../util/function_ref.h"

namespace vray {

	struct CompLightGridCell {
		Volume volume;
		LightCell::Index index;
	};

	class LightGrid {
		using QueryCallback = FunctionRef<void(entt::entity, LightCell&)>;
	private:
		entt::dense_map<Volume, LightCell, VolumeHash> cells;
		entt::registry& world;
		float cellSize;

		void remove(entt::entity entity, bool fialRemoval);

	public:
		LightGrid(entt::registry& world, float cellSize);

		void insert(entt::entity entity);
		void remove(entt::entity entity);
		void update(entt::entity entity);
		void querySphere(const glm::vec3& posision, float radius, QueryCallback callback);

		void insert(entt::entity entity, const Volume& volume);

		float getCellSize() const { return cellSize; }
	};

}