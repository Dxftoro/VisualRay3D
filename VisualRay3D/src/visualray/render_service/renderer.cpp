#include "vrpch.h"
#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <thirdparty/glm/gtx/string_cast.hpp>
#include <thirdparty/glm/gtc/matrix_transform.hpp>

#include "renderer.h"
#include "vertex_array.h"
#include "renderer_callbacks.h"

#include "../world/components.h"

#include "logservice.h"

namespace vray {

	Renderer::Renderer(CameraSystem& _cameraSystem) : cameraSystem(_cameraSystem) {
		if (!gladLoadGL()) {
			VR_ENGINE_LOGERROR("Can't load OpenGL");
			throw std::runtime_error("Can't load OpenGL!");
		}

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(rendererDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		glDisable(GL_DEBUG_OUTPUT);
	}

	Renderer::~Renderer() {

	}

	void Renderer::setClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::update(float deltaTime) {

	}

	void Renderer::submit(const MeshRequest& request) {
		Mesh* mesh = request.renderable->mesh;
		Texture* texture = request.renderable->texture;

		if (!mesh->getHandle().isValid()) {
			managers.meshes.load(mesh);
		}

		if (!texture->getHandle().isValid()) {
			managers.textures.load(texture);
		}

		renderLists.meshes.submit(request);
	}

	void Renderer::submit(const BillboardRequest& request) {
		renderLists.billboards.submit(request);
	}

	void Renderer::flush() {

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