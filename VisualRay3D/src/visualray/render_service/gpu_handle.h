#pragma once

#include "../util/util.h"

namespace vray {

	using GpuHandle_t = unsigned int;

	class MeshId : public Strong<GpuHandle_t> {
	public:
		using Strong<GpuHandle_t>::Strong;

		static constexpr MeshId invalid() { return MeshId(std::numeric_limits<GpuHandle_t>::max()); }
		bool isValid() const { return value != MeshId::invalid().get(); }
	};

	class TextureId : public Strong<GpuHandle_t> {
	public:
		using Strong<GpuHandle_t>::Strong;

		static constexpr MeshId invalid() { return MeshId(0); }
		bool isValid() const { return value != TextureId::invalid().get(); }
	};

}