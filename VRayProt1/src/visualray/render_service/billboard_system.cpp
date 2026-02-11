#include "vrpch.h"
#include "billboard_system.h"
#include "logservice.h"

#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace vray {

	void BillboardVbo::remove(size_t index) {}
	void BillboardVbo::add(entt::entity entity, const CompBillboard& billboard) {}

	//float BillboardSystem::vertexData[] = {
	//	-0.5f, 0.5f, 0.0f,		0.0f, 0.0f,
	//	-0.5f, -0.5f, 0.0f,		0.0f, 1.0f,
	//	0.5f, 0.5f, 0.0f,		1.0f, 0.0f,
	//	0.5f, -0.5f, 0.0f,		1.0f, 1.0f
	//};

	BillboardSystem::BillboardSystem(entt::registry& _world)
		: world(_world), camera(nullptr), texture(nullptr), lastExisting(entt::null), somethingDeleted(false) {

		billboardGroup = BillboardGroup();
		world.on_construct<CompBillboard>().connect<&BillboardSystem::onBillboardAdded>();
		world.on_destroy<CompBillboard>().connect<&BillboardSystem::onBillboardRemoved>();
	}

	BillboardSystem::~BillboardSystem() {
		for (auto& it : batchTable) {
			glDeleteVertexArrays(1, &it.second.vao);
		}
	}

	void BillboardSystem::onBillboardAdded(entt::registry& world, const entt::entity entity) {
		world.emplace<CompBillboardIndex>(entity).index = VR_RENDERER_BILLBOARD_NEW;
	}

	void BillboardSystem::onBillboardRemoved(entt::registry& world, const entt::entity entity) {
		world.emplace<CompBillboardIndex>(entity).deleted = true;
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
		uViewMatrix = program.getUniform("uViewMatrix");

		//glm::vec3 position = { -10.0, 10.0, 0.0 };
	}

	void BillboardSystem::handleDeleted(entt::entity entity, CompBillboardIndex& billboardIndex) {

	}

	void BillboardSystem::update() {
		program.use();
		program.setUniform(uCameraPosition, camera->getPosition());
		program.setUniform(uProjectionMatrix, camera->getProjectionMatrix());
		program.setUniform(uViewMatrix, camera->getViewMatrix());

		/* Or any else value that indicates that we need to handle deleted billboards */
		if (somethingDeleted) {
			for (entt::entity entity : billboardGroup) {

			}
		}

		billboardGroup.each([this](entt::entity, CompBillboardIndex& billboardIndex, CompBillboard& billboard) {
			auto it = batchTable.find(billboard.texture);
			if (it == batchTable.end()) it = createBatch(billboard.texture);

			if (billboard.dirty) return;
			billboard.dirty = false;

			if (billboardIndex.index == VR_RENDERER_BILLBOARD_NEW) {
				billboardIndex.index = it->second.vbo.size();
				it->second.vbo.push(billboard);
			}
			else {
				it->second.vbo.set(billboardIndex.index, billboard); // !!!
			}
		});

		for (auto& it : batchTable) {
			it.first->bind();
				glBindVertexArray(it.second.vao);
				glDrawArrays(GL_POINTS, 0, it.second.vbo.size());
				glBindVertexArray(0);
			it.first->unbind();
		}

		program.unuse();
	}

	BatchTableIterator BillboardSystem::createBatch(Texture* texture) {
		assert(batchTable.find(texture) != batchTable.end() || "Batch already exists!");
		BatchTableIterator it = batchTable.try_emplace(texture).first;

		glGenVertexArrays(1, &it->second.vao);
		glBindVertexArray(it->second.vao);

		it->second.vbo.bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CompBillboard),
			(const void*)offsetof(CompBillboard, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(CompBillboard),
			(const void*)offsetof(CompBillboard, size));

		it->second.vbo.ubind();

		glBindVertexArray(0);

		VR_ENGINE_LOGINFO("Batch created! Capacity: " + STR(it->second.vbo.getCapacity()));
		return it;
	}

}