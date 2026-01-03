#include "vrpch.h"
#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.h"
#include "vertex_array.h"
#include "renderer_callbacks.h"

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

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(rendererDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		camera = new CompCamera(90,
			currentWindow->getWidth(),
			currentWindow->getHeight(), 0.1f, 300.0f);

		material = {
			.ka = glm::vec3(0.1f),
			.kd = glm::vec3(1.0f),
			.ks = glm::vec3(1.0f),
			.shininess = 32.0
		};

		light = {
			.position = glm::vec4(3.0f, 15.0f, 0.0f, 1.0f),
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

			uProjectionMatrix	= program.getUniform("projectionMatrix");
			uViewMatrix			= program.getUniform("viewMatrix");
			uModelMatrix		= program.getUniform("modelMatrix");
			uNormalMatrix		= program.getUniform("normalMatrix");

			uboLight = program.createUniformBuffer("LightData", &light, sizeof(light));
			uboMaterial = program.createUniformBuffer("MaterialData", &material, sizeof(material));

			//glBindBuffer(GL_UNIFORM_BUFFER, uboMaterial.getHandle());
			//	char* lightData = (char*)glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_ONLY);
			//	char* lsData = lightData		// ka
			//		+ sizeof(glm::vec3) + 4 +	// kd
			//		+ sizeof(glm::vec3) + 4 +	// ks
			//		+ sizeof(glm::vec3);		// sh
			//	float* ls = (float*)lsData;
			//	VR_ENGINE_LOGINFO("Light la: " + std::to_string(*ls));

			//	glUnmapBuffer(GL_UNIFORM_BUFFER);
			//glBindBuffer(GL_UNIFORM_BUFFER, 0);

			uDebugProjectionMatrix	= debugProgram.getUniform("projectionMatrix");
			uDebugViewMatrix		= debugProgram.getUniform("viewMatrix");
		}
		catch (std::runtime_error exc) {
			VR_ENGINE_LOGERROR(exc.what());
			std::terminate();
		}

		//uboLight.printUniformData();
		//uboMaterial.printUniformData();

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

		//VR_ENGINE_LOGWARN("Setting data to UBOs");
		//uboMaterial.setData(&material, sizeof(material));

	}

	Renderer::~Renderer() {
		if (camera && initialCamera) delete camera;
	}

	void Renderer::clear() {
		glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::update(float deltaTime) {
		try {
			program.use();
			program.setUniform(uProjectionMatrix, camera->getProjectionMatrix());
			program.setUniform(uViewMatrix, camera->getViewMatrix());

			//uboLight.setData(&light, sizeof(light));

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
	}

	void Renderer::submit(RenderRequest&& request) {
		renderQueue.push(request);
	}

	void Renderer::flush() {
		const glm::mat4& viewMatrix = camera->getViewMatrix();

		while (!renderQueue.empty()) {
			RenderRequest& request = renderQueue.front();
			CompTransform* transform = request.transform;

			if (transform->isDirty()) {
				transform->setNormalMatrix(
					glm::mat3(
						glm::transpose(
							glm::inverse(viewMatrix * transform->getTransformMatrix())))
				);
			}

			program.setUniform(uModelMatrix, transform->getTransformMatrix());
			program.setUniform(uNormalMatrix, transform->getNormalMatrix());

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