#pragma once
#include "gpu_resource_managers.h"
#include "render_lists.h"
#include "glsl_program.h"
#include "../thirdparty/glm/glm.hpp"

namespace vray {

	struct PipelineContext {
		GlslProgram program;
		GlslUniform uProjectionMatrix;
		GlslUniform uViewMatrix;
		GlslUniform uModelMatrix;
		GlslUniform uNormalMatrix;
		GlslUniformBuffer uboLight;
		GlslUniformBuffer uboMaterial;
	};

	class MeshSystem {
	private:
		GpuResourceManagers& managers;
		MeshList& meshList;
		PipelineContext ctx;

	public:
		MeshSystem(GpuResourceManagers& managers, MeshList& _meshList);

		void update(const glm::mat4& projection, const glm::mat4& view, bool viewDirty);
	};

}