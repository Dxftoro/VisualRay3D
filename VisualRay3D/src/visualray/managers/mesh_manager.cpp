#include "vrpch.h"
//#include "mesh_manager.h"
//#include "resource_loader.h"
//
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/glm.hpp>
//#include <glm/gtx/string_cast.hpp>
//
//#define TINYOBJLOADER_IMPLEMENTATION
//#include "thirdparty/tiny_obj_loader.h"
//
//#include "render_service/rendering_buffer.h"
//#include "render_service/vertex_array.h"
//#include "logservice.h"

namespace vray {

	//Resource& MeshManager::load(const std::string& filename, const std::string& resourceName) {
	//	std::ifstream fin(filename);
	//	if (!fin) {
	//		throw std::runtime_error("\"" + filename + "\" - File not found!");
	//	}

	//	MeshLoader meshLoader;
	//	VertexArray* vertexArray = (VertexArray*)meshLoader.load(filename);
	//	fin.close();

	//	std::string meshName = resourceName == "" ? filename : resourceName;

	//	auto result = meshMap.emplace(meshName, Mesh(vertexArray));
	//	return result.first->second;
	//}

	//Resource& MeshManager::get(const std::string& resourceName) {
	//	std::unordered_map<std::string, Mesh>::iterator it = meshMap.find(resourceName);
	//	if (it != meshMap.end()) return it->second;
	//	else return load(resourceName);
	//}

}