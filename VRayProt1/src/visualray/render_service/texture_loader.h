#pragma once
#include "vrpch.h"
#include "kernel.h"

#define VR_RGB						0x1907
#define VR_RGBA						0x1908
#define VR_TEXTURE_UNSIGNED_BYTE	0x1401

namespace vray {

	enum class TextureType : GLenum {
		TEXTURE_2D = 0x0DE1,
		TEXTURE_3D = 0x806F
	};

	enum class TextureParamName : GLenum {
		TEXTURE_WRAP_S = 0x2802,
		TEXTURE_WRAP_T = 0x2803,
		TEXTURE_MIN_FILTER = 0x2801,
		TEXTURE_MAG_FILTER = 0x2800
	};

	enum class TextureParam : GLint {
		LINEAR = 0x2600,
		NEAREST = 0x2601,
		REPEAT = 0x2901
	};

	struct TextureContext {
		GLuint textureId;
		int width;
		int height;
		int colorChannelCount;
		unsigned char* colorData;

		TextureContext();
		~TextureContext();
	};

	class TextureLoader;

	class Texture {
		friend TextureLoader;
	private:
		GLuint type;
		GLuint handle;

		Texture(TextureType _type, GLuint textureId) : handle(textureId), type((GLuint)_type) {}

	public:
		virtual void bind();
		virtual void unbind();
	};

	class TextureLoader {
	private:
		TextureType type;
		TextureParam	textureWrapS, textureWrapT, 
						textureMinFilter, textureMagFilter;

		std::vector<GLuint> textureIds;

	public:
		TextureLoader(TextureType type, 
			TextureParam _textureWrapS, 
			TextureParam _textureWrapT,
			TextureParam _textureMinFilter,
			TextureParam _textureMagFilter);

		~TextureLoader();

		Texture load(const std::string& filename);
	};

}