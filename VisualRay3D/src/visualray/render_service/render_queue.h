#pragma once
#include <vector>

namespace vray {

	template <typename T>
	class RenderList {
	private:
		std::vector<T> list;

	public:
		RenderList() = default;
		RenderList(size_t toReserve) { list.reserve(toReserve); }

		size_t size() const { return list.size(); }
		bool isEmpty() const { return list.empty(); }

		auto begin() { return list.begin(); }
		auto end() { return list.end(); }

		void submit(const T& request) { list.push_back(request); }
		void clear() { list.clear(); }
	};

}