#pragma once

#include "../util/util.h"

namespace vray {

	using GpuHandle_t = unsigned int;

	class GpuHandle : public Strong<GpuHandle_t> {
	public:
		using Strong<GpuHandle_t>::Strong;

		static constexpr GpuHandle invalid() { return GpuHandle(std::numeric_limits<GpuHandle_t>::max()); }
		bool isValid() const { return value != invalid().get();  }
	};

}