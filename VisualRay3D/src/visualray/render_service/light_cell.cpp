#include "vrpch.h"
#include "light_cell.h"

namespace vray {

	LightCell::LightCell() : last(0) {
		for (Index i = 0; i < capacity(); i++) lights[i] = entt::null;
	}

	void LightCell::set(entt::entity entity, Index index) {
		lights[index] = entity;
	}

	void LightCell::push(entt::entity entity) {
		assert(last < capacity() && "Cell if full!");
		lights[last] = entity;
		last++;
	}

}