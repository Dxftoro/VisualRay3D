#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "managers/resource.h"

namespace vray {

	struct VRAYLIB CompName {
		std::string name;
	};

	struct VRAYLIB CompTransform {
	private:
		/* TODO: Remake as bit-mask */
		mutable bool dirty, sync;

		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
		glm::mat4 transformMatrix;
		glm::mat3 normalMatrix;

	public:
		CompTransform()
			: position(0.0f), rotation(glm::identity<glm::quat>()), scale(1.0f),
			  transformMatrix(1.0f), normalMatrix(1.0f), dirty(true), sync(false) {}

		CompTransform(
			const glm::vec3& _position,
			const glm::quat& _rotation,
			const glm::vec3& _scale)
			: position(_position), rotation(_rotation), scale(_scale),
			transformMatrix(1.0f), normalMatrix(1.0f), dirty(true), sync(false) {}

		const glm::vec3& getPosition() const { return position; }
		const glm::quat& getRotation() const { return rotation; }
		const glm::vec3& getScale() const { return scale; }
		const glm::mat4& getTransformMatrix();
		const glm::mat3& getNormalMatrix() const;
		bool isDirty() const { return dirty; };
		bool isSync() const { return sync; }

		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);
		void setRotation(const glm::quat& rotation);
		void setNormalMatrix(const glm::mat3 normalMatrix);
		void setScale(const glm::vec3& scale);
		void setSync(bool sync) { this->sync = sync; };
	};

	struct VRAYLIB CompRenderable {
		Mesh* mesh;
		Texture* texture;
		CompRenderable(Mesh* _mesh, Texture* _texture)
			: mesh(_mesh), texture(_texture) {}
	};

	struct VRAYLIB CompCamera {
	private:
		bool active;

		/* Observing data */
		glm::mat4 viewMatrix, projectionMatrix;

		/* Camera body data */
		glm::vec3 position, rotation;

		void updateMatrices();

	public:
		CompCamera(float fov, float width, float height, float near, float far);
		~CompCamera() {}

		void setViewMatrix(const glm::mat4& viewMatrix);
		void setProjectionMatrix(const glm::mat4& projectionMatrix) { this->projectionMatrix = projectionMatrix; }
		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);

		const glm::mat4& getViewMatrix() const { return viewMatrix; }
		const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
		const glm::vec3& getPosition() const { return position; }
		const glm::vec3& getRotation() const { return rotation; }

		bool isActive() const { return active; }
	};

	struct VRAYLIB CompHitbox {
		enum class ShapeType : short int { BOX, CAPSULE, SPHERE } shapeType;
		enum class PhysType : short int { STATIC, DYNAMIC, KINEMATIC } physType;
		glm::vec3 size;
		float radius;
		float mass;
	};

	struct VRAYLIB CompVisualMaterial {
		alignas(16) glm::vec3 ka;
		alignas(16) glm::vec3 kd;
		alignas(16) glm::vec3 ks;
		float shininess;
	};

	struct VRAYLIB CompPointLight {
		glm::vec4 position;
		alignas(16) glm::vec3 la;
		alignas(16) glm::vec3 ld;
		alignas(16) glm::vec3 ls;
	};
	struct VRAYLIB CompPointLightIndex {
		int index = -1;
		bool dirty = true;
		bool deleted = false;
	};

}