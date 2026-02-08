#include "vrpch.h"
#include "billboard_system.h"
#include "logservice.h"

#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "glsl_flexible_buffer.h"

namespace vray {

	BillboardSystem::BillboardSystem(entt::registry& _world)
		: world(_world), camera(nullptr), texture(nullptr), vao(0), vbo(0), first(true) {
		billboardGroup = world.group<CompBillboard>();
	}

	BillboardSystem::~BillboardSystem() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	void onBillboardAdded(entt::registry& world, const entt::entity entity) {}
	void onBillboardUpdated(entt::registry& world, const entt::entity entity) {}
	void onBillboardRemoved(entt::registry& world, const entt::entity entity) {}

	void BillboardSystem::init(CompCamera* camera) {
		this->camera = camera;

		program.compileShader("shaders\\billboard.vert", ShaderType::VERTEX);
		program.compileShader("shaders\\billboard.geom", ShaderType::GEOMETRY);
		program.compileShader("shaders\\billboard.frag", ShaderType::FRAGMENT);
		program.link();
		program.validate();

		uCameraPosition = program.getUniform("uCameraPosition");
		uProjectionMatrix = program.getUniform("uProjectionMatrix");
		uViewMatrix = program.getUniform("uViewMatrix");

		glm::vec3 position = { -10.0, 10.0, 0.0 };

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(position), &position, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(position), (const void*)0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BillboardSystem::update() {
		program.use();

		//VR_ENGINE_LOGINFO(glm::to_string(camera->getPosition()));
		program.setUniform(uCameraPosition, camera->getPosition());
		program.setUniform(uProjectionMatrix, camera->getProjectionMatrix());
		program.setUniform(uViewMatrix, camera->getViewMatrix());

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

	void BillboardSystem::testFlexibleBuffer() const {
		struct Data {
			int field1;
			int field2;
			float field3;
			Data() = default;
		};

		GlslFlexibleBuffer<GL_ARRAY_BUFFER, Data> buffer(GlslUsage::STREAM_DRAW);
		
		size_t capacity = buffer.getCapacity();
		for (int i = 0; i <= capacity * 3; i++) {
			//VR_ENGINE_LOGIMPORTANT("Index is: " + std::to_string(i));
			buffer.push({});
		}

		VR_ENGINE_LOGIMPORTANT("Buffer size is: " + std::to_string(buffer.size()));
		VR_ENGINE_LOGIMPORTANT("Buffer capacity is: " + std::to_string(buffer.getCapacity()));

		size_t newCapacity = buffer.getCapacity();
		for (int i = 0; i < capacity * 2 + 2; i++) {
			//VR_ENGINE_LOGIMPORTANT("Going to pop index: " + std::to_string(buffer.size() - 1));
			buffer.pop();
		}

		VR_ENGINE_LOGIMPORTANT("Buffer size is: " + std::to_string(buffer.size()));
		VR_ENGINE_LOGIMPORTANT("Buffer capacity is: " + std::to_string(buffer.getCapacity()));

		//while (!buffer.isEmpty()) {
		//	buffer.pop();
		//}

		//VR_ENGINE_LOGIMPORTANT("Buffer size is: " + std::to_string(buffer.size()));
		//VR_ENGINE_LOGIMPORTANT("Buffer capacity is: " + std::to_string(buffer.getCapacity()));
	}

}