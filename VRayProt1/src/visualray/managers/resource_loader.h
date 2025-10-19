#pragma once
#include "vrpch.h"
#include "kernel.h"

namespace vray {

	class IResourceLoader {
	public:
		virtual void* load(std::ifstream& file) = 0;
	};

	class VertexBuffer;

	class MeshLoader : public IResourceLoader {
	private:
		size_t meshSize;
		float latestTimeTook;

		void parseVertexData(std::istringstream& iss, std::vector<float>& vertexData);
		void parseFaces(std::istringstream& iss, std::vector<int>& elements);

	public:
		MeshLoader() : meshSize(0), latestTimeTook(0) {}

		void* load(std::ifstream& file) override;
		void* load(const std::string& filename);

		size_t getMeshSize() const { return meshSize; }
		float getLatestTimeTook() const { return latestTimeTook; };
	};

}