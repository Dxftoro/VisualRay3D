#include "vrpch.h"
#include "billboard_system.h"
#include "camera_system.h"
#include "logservice.h"

#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <thirdparty/glm/gtx/string_cast.hpp>

namespace vray {

	void BillboardVbo::remove(size_t index) {
		if (!index && size() == 1) {
			pop();
			return;
		}

		if (index == size() - 1) {
			pop();
			cache = get(size() - 1);
		}
		else if (index >= 0 && index < size() - 1) {
			set(index, cache);
			pop();
			cache = get(size() - 1);
		}
	}

	void BillboardVbo::add(const CompBillboardData& billboard) {
		push(billboard);
		cache = billboard;
	}

	void BillboardVbo::update(size_t index, const CompBillboardData& billboard) {
		set(index, billboard);
		if (index == size() - 1) cache = billboard;
	}

	//float BillboardSystem::vertexData[] = {
	//	-0.5f, 0.5f, 0.0f,		0.0f, 0.0f,
	//	-0.5f, -0.5f, 0.0f,		0.0f, 1.0f,
	//	0.5f, 0.5f, 0.0f,		1.0f, 0.0f,
	//	0.5f, -0.5f, 0.0f,		1.0f, 1.0f
	//};

	BillboardSystem::BillboardSystem(entt::registry& _world)
	:	world(_world),
		cameraSystem(nullptr),
		texture(nullptr),
		lastExisting(entt::null),
		somethingDeleted(false) {

		billboardGroup = world.group<CompBillboardIndex>(entt::get<CompBillboardData>);
		world.on_construct<CompBillboard>().connect<&BillboardSystem::onBillboardAdded>();
		world.on_destroy<CompBillboard>().connect<&BillboardSystem::onBillboardRemoved>(this);
	}

	BillboardSystem::~BillboardSystem() {
		for (auto& it : batchTable) {
			glDeleteVertexArrays(1, &it.second.vao);
		}
	}

	void BillboardSystem::onBillboardAdded(entt::registry& world, const entt::entity entity) {
		world.emplace<CompBillboardData>(entity);
		world.emplace<CompBillboardIndex>(entity).index = VR_RENDERER_BILLBOARD_NEW;
	}

	void BillboardSystem::onBillboardRemoved(entt::registry& world, const entt::entity entity) {
		auto [billboard, billboardIndex] = world.get<CompBillboardData, CompBillboardIndex>(entity);
		batchTable[billboard.getTexture()].vbo.remove(billboardIndex.index);
		world.erase<CompBillboardIndex>(entity);
		world.erase<CompBillboardData>(entity);
	}

	void BillboardSystem::init(CameraSystem* cameraSystem) {
		this->cameraSystem = cameraSystem;

		program.compileShader("shaders\\billboard.vert", ShaderType::VERTEX);
		program.compileShader("shaders\\billboard.geom", ShaderType::GEOMETRY);
		program.compileShader("shaders\\billboard.frag", ShaderType::FRAGMENT);
		program.link();
		program.validate();

		uCameraPosition = program.getUniform("uCameraPosition");
		uProjectionView = program.getUniform("uProjectionView");

		//glm::vec3 position = { -10.0, 10.0, 0.0 };
	}

	void BillboardSystem::update() {
		program.use();
		program.setUniform(uCameraPosition, cameraSystem->getActiveCamera()->getPosition());
		program.setUniform(uProjectionView, cameraSystem->getProjectionViewCache());

		billboardGroup.each([this](entt::entity, CompBillboardIndex& billboardIndex, CompBillboardData& billboard) {
			auto it = batchTable.find(billboard.getTexture());
			if (it == batchTable.end()) it = createBatch(billboard.getTexture());

			if (!billboard.isDirty()) return;
			billboard.setDirty(false);

			it->second.vbo.bind();
			if (billboardIndex.index == VR_RENDERER_BILLBOARD_NEW) {
				VR_ENGINE_LOGIMPORTANT("New billboard detected!");
				billboardIndex.index = it->second.vbo.size();
				it->second.vbo.add(billboard);
			}
			else {
				it->second.vbo.update(billboardIndex.index, billboard); // !!!
			}
			it->second.vbo.ubind();
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CompBillboardData),
			(const void*)CompBillboardData::getOffsetOfPosition());

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(CompBillboardData),
			(const void*)CompBillboardData::getOffsetOfSize());

		it->second.vbo.ubind();

		glBindVertexArray(0);

		VR_ENGINE_LOGINFO("Batch created! Capacity: " + STR(it->second.vbo.getCapacity()));
		return it;
	}

}