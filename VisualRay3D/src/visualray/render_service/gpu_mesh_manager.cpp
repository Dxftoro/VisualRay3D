#include "vrpch.h"
#include "gpu_mesh_manager.h"
#include "vertex_array.h"

namespace vray {

	GpuHandle GpuMeshManager::load(Mesh* mesh) {
		VertexArray* vertexArray = new VertexArray();
		vertexArray->bind();

		BufferLayout layout({
			{ "VertexPosition",		BufferObjectType::FLOAT3 },
			{ "VertexNormal",		BufferObjectType::FLOAT3 },
			{ "VertexTexCoords",	BufferObjectType::FLOAT2 }
		});

		auto& vertexData = mesh->getVertexData();
		auto& elements = mesh->getElements();

		VertexBuffer* vertexBuffer = new VertexBuffer(vertexData.data(),
			vertexData.size() * sizeof(float), layout);

		ElementBuffer* elementBuffer = new ElementBuffer(elements.data(), elements.size() * sizeof(int));

		vertexArray->addVertexBuffer(vertexBuffer);
		vertexArray->setElementBuffer(elementBuffer);

		vertexArray->unbind();

		GpuMesh gpuMesh = { vertexArray };
		GpuHandle handle = submit(gpuMesh);
		mesh->setHandle(handle);
		return handle;
	}

	void GpuMeshManager::unload(GpuHandle handle) {
		auto& gpuMesh = get(handle);
		delete gpuMesh.vertexArray;
		gpuMesh.vertexArray = nullptr;
		release(handle);
	}

}