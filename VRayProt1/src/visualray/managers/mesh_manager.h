#pragma once
#include "vrpch.h"
#include "kernel.h"
#include "resource_manager.h"

namespace vray {

	class VertexArray;

	class Mesh : public Resource {
	private:
		VertexArray* vertexArray;
		bool copy;

	public:
		Mesh(const std::string& resourceName, VertexArray* _vertexArray)
			: Resource(resourceName), vertexArray(_vertexArray), copy(false) {}
		~Mesh();

		Mesh(const Mesh&);
		Mesh& operator=(const Mesh&);

		VertexArray* getVertexArray() const { return vertexArray; }
		bool isACopy() const { return copy; }
	};

	class MeshManager : public IResourceManager {
	private:
		std::unordered_map<std::string, Mesh> meshMap;

	public:
		MeshManager() {}
		~MeshManager() {}

		Resource& load(const std::string& filename, const std::string& resourceName = "") override;
		Resource& get(const std::string& resourceName) override;
		void clear() { meshMap.clear(); }
	};

}