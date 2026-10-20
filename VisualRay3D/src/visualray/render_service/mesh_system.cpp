#pragma once
#include "vrpch.h"
#include "mesh_system.h"
#include "vertex_array.h"
#include "../world/components.h"

#include <glad/glad.h>

namespace vray {

	MeshSystem::MeshSystem(GpuResourceManagers& _managers, MeshList& _meshList)
	:	managers(_managers), meshList(_meshList) {
		
		CompRenderable::Material material = {
			.ka = glm::vec3(0.1f),
			.kd = glm::vec3(1.0f),
			.ks = glm::vec3(1.0f),
			.shininess = 32.0
		};

		try {
			ctx.program.compileShader("shaders/phong.vert", ShaderType::VERTEX);
			ctx.program.compileShader("shaders/phong.frag", ShaderType::FRAGMENT);
			ctx.program.link();
			ctx.program.validate();

			ctx.uProjectionMatrix = ctx.program.getUniform("projectionMatrix");
			ctx.uViewMatrix = ctx.program.getUniform("viewMatrix");
			ctx.uModelMatrix = ctx.program.getUniform("modelMatrix");
			ctx.uNormalMatrix = ctx.program.getUniform("normalMatrix");

			ctx.uboMaterial = ctx.program.createUniformBuffer("MaterialData", &material, sizeof(material));
		}
		catch (std::runtime_error& exc) {
			VR_ENGINE_LOGERROR(exc.what());
			std::terminate();
		}
	}

	void MeshSystem::update(const glm::mat4& projection, const glm::mat4& view, bool viewDirty) {
		ctx.uboMaterial.bind();

		for (auto& request : meshList) {
			CompRenderable* renderable = request.renderable;
			CompTransformMatrices* matrices = request.matrices;
			CompTransform* transform = request.transform;

			ctx.program.setUniform(ctx.uModelMatrix, matrices->transform);
			ctx.program.setUniform(ctx.uNormalMatrix, matrices->transform);
			ctx.uboMaterial.setData(&(renderable->material), sizeof(CompRenderable::Material));

			GpuHandle meshHandle = renderable->mesh->getHandle();
			GpuHandle textureHandle = renderable->texture->getHandle();

			if (!meshHandle.isValid()) {
				meshHandle = managers.meshes.load(renderable->mesh);
			}
			if (!textureHandle.isValid()) {
				textureHandle = managers.textures.load(renderable->texture);
			}

			GpuMesh& gpuMesh = managers.meshes.get(meshHandle);
			GpuTexture& gpuTexture = managers.textures.get(textureHandle);

			glBindTexture(GL_TEXTURE_2D, gpuTexture.handle);
			gpuMesh.vertexArray->bind();
			gpuMesh.vertexArray->drawElements(DrawMode::TRIANGLES);
			gpuMesh.vertexArray->unbind();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

}