#pragma once

#include "gpu_resource_manager.h"
#include "gpu_texture.h"
#include "../managers/resource.h"

namespace vray {

	class GpuTextureManager : public GpuResourceManager<GpuTexture, Texture> {
	public:
		GpuHandle load(Texture* mesh) override;
		void unload(GpuHandle handle) override;
	};

}
