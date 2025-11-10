#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "managers/resource.h"

namespace vray {

	struct VRAYLIB CompTransform {
	private:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
		glm::mat4 transformMatrix;
		glm::mat3 normalMatrix;
		mutable bool dirty;

	public:
		CompTransform()
			: position(0.0f), rotation(glm::identity<glm::quat>()), scale(1.0f),
			  transformMatrix(1.0f), normalMatrix(1.0f), dirty(true) {}

		CompTransform(
			const glm::vec3& _position,
			const glm::quat& _rotation,
			const glm::vec3& _scale)
			: position(_position), rotation(_rotation), scale(_scale),
			transformMatrix(1.0f), normalMatrix(1.0f), dirty(true) {}

		const glm::vec3& getPosition() const { return position; }
		const glm::quat& getRotation() const { return rotation; }
		const glm::vec3& getScale() const { return scale; }
		const glm::mat4& getTransformMatrix();
		const glm::mat3& getNormalMatrix();

		void setPosition(glm::vec3 position);
		void setRotation(glm::vec3 rotation);
		void setScale(glm::vec3 scale);
	};

	struct VRAYLIB CompRenderable {
		Mesh* mesh;
		Texture* texture;
		CompRenderable(Mesh* _mesh, Texture* _texture)
			: mesh(_mesh), texture(_texture) {}
	};

}