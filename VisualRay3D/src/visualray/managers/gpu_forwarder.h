#pragma once

#include "resource_manager.h"
#include "resource.h"
#include "../render_service/gpu_handle.h"

namespace vray {

	template <typename T>
	class GpuForwarder : public ResourceManager<T> {
	private:
		std::vector<GpuHandle> freed;

	public:
		GpuForwarder() {}

		void unload(T* resource) { freed.push_back(resource->getHandle()); }
		std::vector<GpuHandle>& getFreed() { return freed; }
	};

	using TextureManager = GpuForwarder<Texture>;
	using MeshManager = GpuForwarder<Mesh>;

}