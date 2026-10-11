#include "vrpch.h"
#include "gpu_texture_manager.h"
#include <glad/glad.h>

namespace vray {

	GpuHandle GpuTextureManager::load(Texture* texture) {
		GpuTexture gpu;

		glGenTextures(1, &gpu.handle);
		glBindTexture(GL_TEXTURE_2D, gpu.handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->getWidth(), texture->getHeight(), 0,
			texture->getChannelCount() == 4 ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE, texture->getPixels().data());
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		gpu.width = texture->getWidth();
		gpu.height = texture->getHeight();
		gpu.cCount = texture->getChannelCount();
		
		GpuHandle handle = submit(gpu);
		texture->setHandle(handle);
		return handle;
	}

	void GpuTextureManager::unload(GpuHandle handle) {
		auto& texture = get(handle);
		glDeleteTextures(1, &texture.handle);
		release(handle);
	}

}