#pragma once
#include <vector>
#include <queue>
#include "gpu_handle.h"
#include "gpu_texture.h"
#include "../managers/resource.h"

namespace vray {

	template <typename T, typename F>
	class GpuResourceManager {
	protected:
		std::vector<T> resources;
		std::queue<GpuHandle> indicies;
		
		GpuHandle submit(const T& gpuResource) {
			GpuHandle handle = GpuHandle::invalid();

			if (!indicies.empty()) {
				handle = GpuHandle(indicies.front().get());
				resources[handle.get()] = gpuResource;
				indicies.pop();
			}
			else {
				resources.push_back(gpuResource);
				handle = GpuHandle(static_cast<GpuHandle_t>(resources.size() - 1));
			}

			return handle;
		}

		void release(GpuHandle handle) {
			assert(handle.isValid() && "Invalid GPU handle");
			indicies.push(handle);
		}

	public:
		GpuResourceManager() = default;

		virtual GpuHandle load(F* resource) = 0;
		virtual void unload(GpuHandle handle) = 0;

		T& get(GpuHandle handle) {
			assert(handle.isValid() && "Invalid GPU handle");
			return resources[handle.get()];
		}
	};

}