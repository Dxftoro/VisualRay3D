#include "vrpch.h"
#include "billboard_system.h"

#include <glad/glad.h>

namespace vray {

	BillboardSystem::BillboardSystem(entt::registry& _world)
		: world(_world), camera(nullptr), texture(nullptr), vao(0), vbo(0) {
		billboardGroup = world.group<CompBillboard>();
	}

	void BillboardSystem::init(CompCamera* camera) {
		this->camera = camera;

		program.compileShader("billboard.vert", ShaderType::VERTEX);
		program.compileShader("billboard.geom", ShaderType::GEOMETRY);
		program.compileShader("billboard.frag", ShaderType::FRAGMENT);
		program.link();
		program.validate();

		program.getUniform("uCameraPosition");
		program.getUniform("uProjectionMatrix");

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

		program.setUniform(uCameraPosition, camera->getPosition());
		program.setUniform(uProjectionMatrix, camera->getProjectionMatrix());

		texture->bind();

		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);

		texture->unbind();

		program.unuse();
	}

}