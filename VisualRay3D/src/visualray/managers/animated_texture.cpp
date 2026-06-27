#include "vrpch.h"
#include "resource.h"
#include "logservice.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace vray {

	unsigned int createTextureHandle(stbi_uc* data, int width, int height, int cCount) {
		unsigned int handle = 0;

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			cCount == 4 ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		return handle;
	}

	AnimatedTexture::AnimatedTexture(const std::string& filename) : fps(0) {
		std::ifstream fin(filename, std::ios::binary);
		if (!fin) {
			throw std::runtime_error("Can't open GIF \"" + filename + "\"!");
		}

		fin.seekg(0, std::ios::end);
		std::streamsize sourceBufferSize = fin.tellg();
		fin.seekg(0, std::ios::beg);

		std::vector<stbi_uc> sourceBuffer(sourceBufferSize);
		fin.read((char*)sourceBuffer.data(), sourceBuffer.size());
		fin.close();

		int* delays = nullptr;
		int frameCount = 0;
		int reqCount = 4;

		stbi_uc* data = stbi_load_gif_from_memory(
			sourceBuffer.data(),
			sourceBuffer.size(),
			&delays,
			&width, &height,
			&frameCount,
			&cCount, reqCount);

		if (data == nullptr || cCount < 4 || !frameCount) {
			std::string errorMessage = "Error when loading GIF: " + filename;
			VR_LOGERROR(errorMessage);
			throw std::runtime_error(errorMessage);
		}

		int frameStride = width * height * reqCount;
		handles.resize(frameCount);

		for (int i = 0; i < handles.size(); i++) {
			stbi_uc* frameOffset = data + (i * frameStride);
			handles[i] = createTextureHandle(frameOffset, width, height, cCount);
			fps += (delays[i] / 1000.0f);
		}
		
		fps = 1.0f / (fps / (float)frameCount);
		VR_ENGINE_LOGIMPORTANT("Loaded GIF FPS: " + STR(fps));

		std::free(data);
		std::free(delays);
	}

	void AnimatedTexture::bind() {

	}

	void AnimatedTexture::unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}