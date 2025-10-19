#include "vrpch.h"
#include "texture_loader.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb_image.h"

namespace vray {

	TextureContext::TextureContext() {
		memset((void*)this, 0, sizeof(*this));
	}

	TextureContext::~TextureContext() {
		if (colorData) stbi_image_free(colorData);
	}

	void Texture::bind() { glBindTexture(type, handle); }
	void Texture::unbind() { glBindTexture(type, handle); }

	TextureLoader::TextureLoader(TextureType _type,
		TextureParam _textureWrapS,
		TextureParam _textureWrapT,
		TextureParam _textureMinFilter,
		TextureParam _textureMagFilter)
	:	type(_type), textureWrapS(_textureWrapS), textureWrapT(_textureWrapT),
		textureMinFilter(_textureMinFilter), textureMagFilter(_textureMagFilter) {
	}

	Texture TextureLoader::load(const std::string& filename) {
		TextureContext textureContext;
		textureContext.colorData = stbi_load(filename.c_str(), 
			&textureContext.width,
			&textureContext.height,
			&textureContext.colorChannelCount, 0);

		if (textureContext.colorData == nullptr) {
			throw std::runtime_error("Can't load texture \"" + filename + "\"");
		}

		GLuint textureId = 0;
		GLenum castedType = (GLenum)type;

		glGenTextures(1, &textureId);
		glBindTexture(castedType, textureId);

		glTexParameteri(castedType, (GLenum)TextureParamName::TEXTURE_WRAP_S, (GLint)textureWrapS);
		glTexParameteri(castedType, (GLenum)TextureParamName::TEXTURE_WRAP_T, (GLint)textureWrapT);
		glTexParameteri(castedType, (GLenum)TextureParamName::TEXTURE_MIN_FILTER, (GLint)textureMinFilter);
		glTexParameteri(castedType, (GLenum)TextureParamName::TEXTURE_MAG_FILTER, (GLint)textureMagFilter);

		glTexImage2D(castedType, 0, VR_RGBA,
			textureContext.width, textureContext.height, 0,
			textureContext.colorChannelCount == 4 ? VR_RGBA : VR_RGB,
			VR_TEXTURE_UNSIGNED_BYTE, textureContext.colorData);

		glBindTexture(castedType, 0);

		textureIds.push_back(textureId);
		return Texture(type, textureId);
	}

	TextureLoader::~TextureLoader() {
		for (GLuint id : textureIds) glDeleteTextures(1, &id);
	}
}
