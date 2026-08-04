#include "vrpch.h"
#include "resource.h"

#include <limits.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtx/string_cast.hpp>
#include <thirdparty/glm/gtx/hash.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "render_service/rendering_buffer.h"
#include "render_service/vertex_array.h"
#include "logservice.h"

namespace vray {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture;

		void writeToBuffer(std::vector<float>& vertexData) const;
		bool operator==(const Vertex& other) const;
	};

	void Vertex::writeToBuffer(std::vector<float>& vertexData) const {
		vertexData.push_back(position.x);
		vertexData.push_back(position.y);
		vertexData.push_back(position.z);

		vertexData.push_back(normal.x);
		vertexData.push_back(normal.y);
		vertexData.push_back(normal.z);

		vertexData.push_back(texture.x);
		vertexData.push_back(texture.y);
	}

	bool Vertex::operator==(const Vertex& other) const {
		return position == other.position
			&& normal == other.normal
			&& texture == other.texture;
	}

}

namespace std {

	template<> struct hash<vray::Vertex> {
		size_t operator()(const vray::Vertex& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^
				(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texture) << 1);
		}
	};

}

namespace vray {

	Mesh::Mesh(const std::string& filename)
	:	Resource(false),
		baseSize(0.0f),
		aabbMin(FLT_MAX),
		aabbMax(-FLT_MAX) {

		std::ifstream fin(filename);
		if (!fin) {
			throw std::runtime_error("Can't open mesh \"" + filename + "\"!");
		}

		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warningMessage, errorMessage;

		bool success = tinyobj::LoadObj(&attributes, &shapes, &materials,
			&warningMessage, &errorMessage, &fin, nullptr, true);

		fin.close();

		if (!success) {
			VR_LOGERROR(errorMessage);
			throw std::runtime_error(errorMessage);
		}
		if (!warningMessage.empty()) VR_LOGWARN(warningMessage);

		std::vector<float> vertexData;
		std::vector<int> elements;
		std::unordered_map<Vertex, int> vertexMap;

		for (const tinyobj::shape_t& shape : shapes) {
			for (const tinyobj::index_t& index : shape.mesh.indices) {
				Vertex vertex;
				vertex.position = {
					attributes.vertices[3 * index.vertex_index],
					attributes.vertices[3 * index.vertex_index + 1],
					attributes.vertices[3 * index.vertex_index + 2]
				};

				if (index.normal_index >= 0) {
					vertex.normal = {
						attributes.normals[3 * index.normal_index],
						attributes.normals[3 * index.normal_index + 1],
						attributes.normals[3 * index.normal_index + 2]
					};
				}
				else {
					vertex.normal = { 1.0f, 1.0f, 1.0f };
				}

				if (index.texcoord_index >= 0) {
					vertex.texture = {
						attributes.texcoords[2 * index.texcoord_index],
						attributes.texcoords[2 * index.texcoord_index + 1]
					};
				}
				else {
					vertex.texture = { 1.0f, 1.0f };
				}

				auto it = vertexMap.find(vertex);
				if (it == vertexMap.end()) {
					aabbMin = glm::min(aabbMin, vertex.position);
					aabbMax = glm::max(aabbMax, vertex.position);
					vertexMap[vertex] = vertexData.size() / 8;
					vertex.writeToBuffer(vertexData);
				}

				elements.push_back(vertexMap[vertex]);
			}
		}

		baseSize = aabbMax - aabbMin; // !!!

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

	Mesh::Mesh(const Mesh& mesh) : Resource(true) {
		this->vertexArray = mesh.vertexArray;
	}

	Mesh& Mesh::operator=(const Mesh& mesh) {
		if (this != &mesh) {
			this->vertexArray = mesh.vertexArray;
			copy = true;
		}
		return *this;
	}

	Mesh::~Mesh() {
		if (!copy) { delete vertexArray; }
	}

}