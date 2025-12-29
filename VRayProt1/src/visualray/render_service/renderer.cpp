#include "vrpch.h"
#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "renderer.h"
#include "vertex_array.h"
#include "world/components.h"

#include "event_service/game_events.h"
#include "event_service/mouse_events.h"

#include "input_codes.h"
#include "input_service/input_service.h"

#include "logservice.h"

namespace vray {

	Renderer::Renderer(Window* _currentWindow)
		: currentWindow(_currentWindow), initialCamera(true) {

		if (!gladLoadGL()) {
			throw std::runtime_error("Can't load OpenGL!");
		}

		camera = new CompCamera(90,
			currentWindow->getWidth(),
			currentWindow->getHeight(), 0.1f, 300.0f);

		material = {
			.ka = glm::identity<glm::vec3>(),
			.kd = glm::identity<glm::vec3>(),
			.ks = glm::identity<glm::vec3>(),
			.shininess = 64
		};

		light = {
			.position = glm::vec4(3.0f, 6.0f, 0.0f, 1.0f),
			.la = glm::vec3(0.03f),
			.ld = glm::vec3(0.6),
			.ls = glm::vec3(1.0)
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

			uLight				= program.getUniform("light");
			uMaterial			= program.getUniform("material");
			uProjectionMatrix	= program.getUniform("projectionMatrix");
			uViewMatrix			= program.getUniform("viewMatrix");
			uModelMatrix		= program.getUniform("modelMatrix");
			uNormalMatrix		= program.getUniform("normalMatrix");

			uDebugProjectionMatrix	= debugProgram.getUniform("projectionMatrix");
			uDebugViewMatrix		= debugProgram.getUniform("viewMatrix");
		}
		catch (std::runtime_error exc) {
			VR_ENGINE_LOGERROR(exc.what());
			std::terminate();
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);

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
	}

	Renderer::~Renderer() {
		if (camera && initialCamera) delete camera;
	}

	void Renderer::clear() {
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::update(float deltaTime) {
		try {
			program.use();
			program.setUniform(uProjectionMatrix, camera->getProjectionMatrix());
			program.setUniform(uViewMatrix, camera->getViewMatrix());
			program.setUniform(uLight, light);
			program.setUniform(uLight, material);

			flush();

			/* UNIFORM ORDER IS CRITICAL!!! */
			debugProgram.use();
			debugProgram.setUniform(uDebugProjectionMatrix, camera->getProjectionMatrix());
			debugProgram.setUniform(uDebugViewMatrix, camera->getViewMatrix());

			glBindVertexArray(debugVao);
			glDrawArrays(GL_LINES, 0, debugVertexCount);
		}
		catch (std::runtime_error exc) {
			VR_ENGINE_LOGERROR(exc.what());
			return;
		}

		int errorCode;
		if ((errorCode = glGetError())) {
			VR_ENGINE_LOGERROR(std::to_string(errorCode));
		}
	}

	void Renderer::submit(RenderRequest&& request) {
		renderQueue.push(request);
	}

	void Renderer::flush() {
		while (!renderQueue.empty()) {
			RenderRequest& request = renderQueue.front();

			program.setUniform(uModelMatrix, request.transform->getTransformMatrix());
			program.setUniform(uNormalMatrix, request.transform->getNormalMatrix());

			VertexArray* vertexArray = request.renderable->mesh->getVertexArray();
			
			request.renderable->texture->bind();
			vertexArray->bind();
			vertexArray->drawElements(static_cast<DrawMode>(request.drawMode));
			vertexArray->unbind();
			request.renderable->texture->unbind();

			renderQueue.pop();
		}
	}

	void Renderer::onEvent(Event& evt) {
		switch (evt.getType()) {
		case EventType::WINDOW_RESIZE: {
			WindowResizeEvent& resizeEvt = dynamic_cast<WindowResizeEvent&>(evt);
			glViewport(0, 0, resizeEvt.getWidth(), resizeEvt.getHeight());
			resizeEvt.dump();
			break;
		}
		default: break;
		}
	}

	void Renderer::setCamera(CompCamera* camera) {
		if (initialCamera) {
			delete this->camera;
			initialCamera = false;
		}
		this->camera = camera;
	}

	void Renderer::updateDebugPrimitives(const std::vector<float>& vertexData,
		const std::vector<int>& elements) {

		glBindBuffer(GL_ARRAY_BUFFER, debugVbo);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
			vertexData.data(), GL_DYNAMIC_DRAW);

		debugVertexCount = elements.size();
	}
}