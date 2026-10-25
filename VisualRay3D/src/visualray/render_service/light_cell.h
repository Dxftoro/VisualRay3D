#pragma once
#include "../thirdparty/EnTT/entt.hpp"

namespace vray {

	class LightCell {
	public:
		using Index = unsigned int;
		static constexpr Index LIGHT_CELL_SIZE = 8;

	private:
		entt::entity lights[LIGHT_CELL_SIZE];
		Index last;

	public:
		LightCell();

		void set(entt::entity entity, Index index);
		void push(entt::entity entity);
		void pop() { assert(last != 0 && "Cell is empty!"); last--; }

		Index size() const { return last; }
		entt::entity get(Index index) const { return lights[index]; }
		constexpr Index capacity() const { return LIGHT_CELL_SIZE; }
	};

}