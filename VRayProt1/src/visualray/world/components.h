#pragma once
#include <glm/glm.hpp>
#include "managers/mesh_manager.h"

namespace vray {

	struct TransformComponent {
	private:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 transformMatrix;
		bool dirty;

	public:
		TransformComponent(
			const glm::vec3& _position,
			const glm::vec3& _rotation,
			const glm::vec3& _scale)
			: position(_position), rotation(_rotation), scale(_scale),
			transformMatrix(1.0f), dirty(true) {}

		glm::vec3& getPosition() { return position; }
		glm::vec3& getRotation() { return rotation; }
		glm::vec3& getScale() { return scale; }
		glm::mat4& getTransformMatrix();
	};

	struct RenderableComponent {
		Mesh mesh;

		RenderableComponent(const Mesh& _mesh) : mesh(_mesh) {}
	};

}