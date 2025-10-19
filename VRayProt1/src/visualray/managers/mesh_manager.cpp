#include "vrpch.h"
#include "mesh_manager.h"
#include "resource_loader.h"

namespace vray {

	Mesh::Mesh(const Mesh& mesh) : Resource(mesh.name) {
		this->vertexArray = vertexArray;
		copy = true;
	}

	Mesh& Mesh::operator=(const Mesh& mesh) {
		if (this != &mesh) {
			this->name = mesh.name;
			this->vertexArray = mesh.vertexArray;
			copy = true;
		}
		return *this;
	}

	Mesh::~Mesh() { if (!copy) delete vertexArray; }

	Resource& MeshManager::load(const std::string& filename, const std::string& resourceName) {
		std::ifstream fin(filename);
		if (!fin) {
			throw std::runtime_error("\"" + filename + "\" - File not found!");
		}

		MeshLoader meshLoader;
		VertexArray* vertexArray = (VertexArray*)meshLoader.load(filename);
		fin.close();

		std::string meshName = resourceName == "" ? filename : resourceName;

		auto result = meshMap.emplace(meshName, Mesh(meshName, vertexArray));
		return result.first->second;
	}

	Resource& MeshManager::get(const std::string& resourceName) {
		std::unordered_map<std::string, Mesh>::iterator it = meshMap.find(resourceName);
		if (it != meshMap.end()) return it->second;
		else return load(resourceName);
	}

}