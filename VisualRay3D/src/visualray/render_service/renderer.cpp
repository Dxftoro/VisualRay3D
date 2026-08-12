#include "vrpch.h"
#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <thirdparty/glm/gtx/string_cast.hpp>
#include <thirdparty/glm/gtc/matrix_transform.hpp>

#include "renderer.h"
#include "vertex_array.h"
#include "renderer_callbacks.h"

#include "input_codes.h"
#include "input_service/input_service.h"

#include "logservice.h"

namespace vray {

	void updateTransform(
		CompRenderable* renderable,
		CompTransform* transform,
		CompTransformMatrices* matrices) {
		matrices->transform = glm::identity<glm::mat4>();
		matrices->transform = glm::translate(matrices->transform, transform->getPosition());
		matrices->transform *= glm::mat4_cast(transform->getRotation());

		glm::vec3 scale = transform->getSize() / renderable->mesh->getBaseSize();
		//VR_ENGINE_LOGIMPORTANT(
		//	glm::to_string(transform->getScale()) + " | " +
		//	glm::to_string(scale));

		matrices->transform = glm::scale(matrices->transform,
			transform->getSize() / renderable->mesh->getBaseSize());

		transform->setDirty(false);
	}

	//void updateTransform(CompTransform* transform, CompTransformMatrices* matrices) {
	//	matrices->transform = glm::identity<glm::mat4>();
	//	matrices->transform = glm::translate(matrices->transform, transform->getPosition());
	//	matrices->transform *= glm::mat4_cast(transform->getRotation());
	//	matrices->transform = glm::scale(matrices->transform, transform->getScale());
	//	transform->setDirty(false);
	//}

	void onTransformAdded(entt::registry& world, const entt::entity entity) {
		//VR_ENGINE_LOGIMPORTANT("Transform added!");

		auto& transform = world.get<CompTransform>(entity);
		auto& matrices = world.emplace<CompTransformMatrices>(entity, CompTransformMatrices(
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat3>()
		));
	}

	void onTransformRemoved(entt::registry& world, const entt::entity entity) {
		world.erase<CompTransformMatrices>(entity);
	}

	Renderer::Renderer(Window* _currentWindow, CameraSystem& cameraSystem, entt::registry& world)
		: currentWindow(_currentWindow), cameraSystem(cameraSystem), lightSystem(world), billboardSystem(world) {

		if (!gladLoadGL()) {
			throw std::runtime_error("Can't load OpenGL!");
		}

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(rendererDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		glDisable(GL_DEBUG_OUTPUT);

		material = {
			.ka = glm::vec3(0.1f),
			.kd = glm::vec3(1.0f),
			.ks = glm::vec3(1.0f),
			.shininess = 32.0
		};

		try {
			program.compileShader("shaders\\phong.vert", ShaderType::VERTEX);
			program.compileShader("shaders\\phong.frag", ShaderType::FRAGMENT);
			program.link();
			program.validate();

			debugProgram.compileShader("shaders\\debug.vert", ShaderType::VERTEX);
			debugProgram.compileShader("shaders\\debug.frag", ShaderType::FRAGMENT);
			debugProgram.link();
			debugProgram.validate();

			uProjectionMatrix	= program.getUniform("projectionMatrix");
			uViewMatrix			= program.getUniform("viewMatrix");
			uModelMatrix		= program.getUniform("modelMatrix");
			uNormalMatrix		= program.getUniform("normalMatrix");

			lightSystem.initBuffer(program);
			uboMaterial = program.createUniformBuffer("MaterialData", &material, sizeof(material));

			uDebugProjectionViewMatrix	= debugProgram.getUniform("projectionViewMatrix");

			billboardSystem.init(&cameraSystem);
		}
		catch (std::runtime_error exc) {
			VR_ENGINE_LOGERROR(exc.what());
			std::terminate();
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_CULL_FACE);

		glGenVertexArrays(1, &debugVao);
		glGenBuffers(1, &debugVbo);

		glBindVertexArray(debugVao);
		glBindBuffer(GL_ARRAY_BUFFER, debugVbo);

		glBufferData(GL_ARRAY_BUFFER, VR_RENDERER_MAX_DEBUG_LINES * sizeof(float) * 3,
			nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
		glBindVertexArray(0);

		debugVertexCount = 0;

		setClearColor(glm::vec4(0.2f));

		world.on_construct<CompTransform>().connect<onTransformAdded>();
		world.on_destroy<CompTransform>().connect<onTransformRemoved>();
	}

	Renderer::~Renderer() {
		/* Another empty destructor */
	}

	void Renderer::setClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::update(float deltaTime) {
		const glm::mat4& projectionViewMatrix = cameraSystem.getProjectionViewCache();

		try {
			program.use();
			program.setUniform(uProjectionMatrix, cameraSystem.getProjectionMatrix());
			program.setUniform(uViewMatrix, cameraSystem.getViewMatrix());

			lightSystem.update();

			flush();

			debugProgram.use();
			debugProgram.setUniform(uDebugProjectionViewMatrix, projectionViewMatrix);

			glBindVertexArray(debugVao);
			glDrawArrays(GL_LINES, 0, debugVertexCount);

			billboardSystem.update(projectionViewMatrix);
		}
		catch (std::runtime_error exc) {
			VR_ENGINE_LOGERROR(exc.what());
			return;
		}
	}

	void Renderer::submit(RenderRequest&& request) {
		renderQueue.push(request);
	}

	void Renderer::flush() {
		const glm::mat4& viewMatrix = cameraSystem.getViewMatrix();

		uboMaterial.bind();

		while (!renderQueue.empty()) {
			RenderRequest& request = renderQueue.front();
			CompTransform* transform = request.transform;
			CompTransformMatrices* matrices = request.matrices;

			if (cameraSystem.getActiveCamera()->isViewDirty() || transform->isDirty()) {
				updateTransform(request.renderable, transform, matrices);
				matrices->normal = glm::mat3(
					glm::transpose(
						glm::inverse(glm::mat3(viewMatrix * matrices->transform)))
				);
			}

			program.setUniform(uModelMatrix, matrices->transform);
			program.setUniform(uNormalMatrix, matrices->normal);
			uboMaterial.setData(&(request.renderable->material),
				sizeof(CompRenderable::Material));

			VertexArray* vertexArray = request.renderable->mesh->getVertexArray();
			
			request.renderable->texture->bind();
			vertexArray->bind();
			vertexArray->drawElements(static_cast<DrawMode>(request.drawMode));
			vertexArray->unbind();
			request.renderable->texture->unbind();

			renderQueue.pop();
		}

		uboMaterial.unbind();

		cameraSystem.getActiveCamera()->setViewDirty(false);
	}

	bool Renderer::onWindowResize(WindowResizeEvent& evt) {
		int width = evt.getWidth();
		int height = evt.getHeight();
		
		if (!width || !height) return true;

		cameraSystem.setProjectionBorders(width, height);
		glViewport(0, 0, width, height);
		return true;
	}

	void Renderer::updateDebugPrimitives(const std::vector<float>& vertexData,
		const std::vector<int>& elements) {

		glBindBuffer(GL_ARRAY_BUFFER, debugVbo);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
			vertexData.data(), GL_DYNAMIC_DRAW);

		debugVertexCount = elements.size();
	}
}