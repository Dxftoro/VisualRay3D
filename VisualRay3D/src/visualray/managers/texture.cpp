#include "vrpch.h"
#include "resource.h"
#include "logservice.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace vray {

	Texture::Texture(const std::string& filename)
		: width(0), height(0), cCount(0), handle(TextureId::invalid().get()) {
		unsigned char* colorData = stbi_load(filename.c_str(), &width, &height, &cCount, 0);

		if (colorData == nullptr) {
			std::string errorMessage = "Error when loading texture: " + filename;
			VR_LOGERROR(errorMessage);
			throw std::runtime_error(errorMessage);
		}
		
		pixels = std::vector<unsigned char>(colorData, colorData + width * height * cCount);
		stbi_image_free(colorData);
	}

}