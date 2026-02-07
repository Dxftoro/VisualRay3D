#include "vrpch.h"
#include "billboard_system.h"
#include "logservice.h"

#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace vray {

	BillboardSystem::BillboardSystem(entt::registry& _world)
		: world(_world), camera(nullptr), texture(nullptr), vao(0), vbo(0), first(true) {
		billboardGroup = world.group<CompBillboard>();
	}

	void BillboardSystem::init(CompCamera* camera) {
		this->camera = camera;

		program.compileShader("shaders\\billboard.vert", ShaderType::VERTEX);
		program.compileShader("shaders\\billboard.geom", ShaderType::GEOMETRY);
		program.compileShader("shaders\\billboard.frag", ShaderType::FRAGMENT);
		program.link();
		program.validate();

		uCameraPosition = program.getUniform("uCameraPosition");
		uProjectionMatrix = program.getUniform("uProjectionMatrix");

		glm::vec3 position = {-4.0, 4.0, 0.0};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(position), &position, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(position), (const void*)0);
		glDisableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BillboardSystem::update() {
		program.use();

		//VR_ENGINE_LOGINFO(glm::to_string(camera->getPosition()));
		program.setUniform(uCameraPosition, camera->getPosition());
		program.setUniform(uProjectionMatrix, camera->getProjectionMatrix());

		if (first) {
			program.printActiveUniforms();
			first = false;
		}

		texture->bind();

		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);

		texture->unbind();

		program.unuse();
	}

}