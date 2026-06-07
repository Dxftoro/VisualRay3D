#include "vrpch.h"
#include "resource.h"

#include <glad/glad.h>
#include <AL/al.h>

#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtx/string_cast.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "thirdparty/tiny_obj_loader.h"

#include <stb/stb_vorbis.h>

#include "render_service/rendering_buffer.h"
#include "render_service/vertex_array.h"
#include "logservice.h"

namespace vray {

	Mesh::Mesh(std::ifstream& fin) : copy(false) {
		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warningMessage, errorMessage;

		bool success = tinyobj::LoadObj(&attributes, &shapes, &materials,
			&warningMessage, &errorMessage, &fin, nullptr, true);

		if (!success) {
			VR_LOGERROR(errorMessage);
			throw std::runtime_error(errorMessage);
		}
		if (!warningMessage.empty()) VR_LOGWARN(warningMessage);

		std::vector<float> vertexData(shapes[0].mesh.indices.size() * 8);
		std::vector<int> elements;
		VR_LOGINFO(std::to_string(vertexData.size()));

		for (const tinyobj::index_t& index : shapes[0].mesh.indices) {
			glm::vec3 vertexPosition(attributes.vertices[3 * index.vertex_index],
				attributes.vertices[3 * index.vertex_index + 1],
				attributes.vertices[3 * index.vertex_index + 2]);

			vertexData[8 * index.vertex_index] = vertexPosition.x;
			vertexData[8 * index.vertex_index + 1] = vertexPosition.y;
			vertexData[8 * index.vertex_index + 2] = vertexPosition.z;

			if (index.normal_index >= 0) {
				vertexData[8 * index.vertex_index + 3] = attributes.normals[3 * index.normal_index];
				vertexData[8 * index.vertex_index + 4] = attributes.normals[3 * index.normal_index + 1];
				vertexData[8 * index.vertex_index + 5] = attributes.normals[3 * index.normal_index + 2];
			}
			else {
				vertexData[8 * index.vertex_index + 3] = 1.0f;
				vertexData[8 * index.vertex_index + 4] = 1.0f;
				vertexData[8 * index.vertex_index + 5] = 1.0f;
			}

			if (index.texcoord_index >= 0) {
				vertexData[8 * index.vertex_index + 6] = attributes.texcoords[2 * index.texcoord_index];
				vertexData[8 * index.vertex_index + 7] = attributes.texcoords[2 * index.texcoord_index + 1];
			}
			else {
				vertexData[8 * index.vertex_index + 6] = 0.0f;
				vertexData[8 * index.vertex_index + 7] = 0.0f;
			}

			elements.push_back(index.vertex_index);
		}

		BufferLayout layout({
			{ "VertexPosition",		BufferObjectType::FLOAT3 },
			{ "VertexNormal",		BufferObjectType::FLOAT3 },
			{ "VertexTexCoords",	BufferObjectType::FLOAT2 }
			});

		vertexArray = new VertexArray();
		vertexArray->bind();

		VertexBuffer* vertexBuffer = new VertexBuffer(vertexData.data(),
			vertexData.size() * sizeof(float), layout);
		ElementBuffer* elementBuffer = new ElementBuffer(elements.data(),
			elements.size() * sizeof(int));

		vertexArray->addVertexBuffer(vertexBuffer);
		vertexArray->setElementBuffer(elementBuffer);

		vertexArray->unbind();
	}

	Mesh::Mesh(const Mesh& mesh) : copy(true) {
		this->vertexArray = mesh.vertexArray;
	}

	Mesh& Mesh::operator=(const Mesh& mesh) {
		if (this != &mesh) {
			this->vertexArray = mesh.vertexArray;
			copy = true;
		}
		return *this;
	}

	Mesh::~Mesh() { if (!copy) delete vertexArray; }

	Texture::Texture(const std::string& filename) {
		int width, height, cCount;
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

	Sound::Sound(const std::string& filename)
		: handle(0), cCount(0), sampleRate(0), sampleCount(0) {
		
		short* samples;
		sampleCount = stb_vorbis_decode_filename(filename.c_str(), &cCount, &sampleRate, &samples);

		if (sampleCount == -1) {
			throw std::runtime_error("Can't load sound!");
		}

		ALenum format = (cCount == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

		alGenBuffers(1, &handle);
		alBufferData(handle, format, samples, sizeof(short) * sampleCount * cCount, sampleRate);

		std::free(samples);
	}

	Sound::~Sound() {
		alDeleteBuffers(1, &handle);
	}

}