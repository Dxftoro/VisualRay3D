#include "vrpch.h"
#include "resource.h"
#include "logservice.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace vray {

	Texture::Texture(const std::string& filename)
		: width(0), height(0), cCount(0) {
		unsigned char* colorData = stbi_load(filename.c_str(), &width, &height, &cCount, 0);

		if (colorData == nullptr) {
			std::string errorMessage = "Error when loading texture: " + filename;
			VR_LOGERROR(errorMessage);
			throw std::runtime_error(errorMessage);
		}

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			cCount == 4 ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE, colorData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(colorData);
	}

	Texture::~Texture() { glDeleteTextures(1, &handle); }

	void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, handle); }

	void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

}