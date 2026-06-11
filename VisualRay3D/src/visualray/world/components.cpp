#include "vrpch.h"
#include "components.h"
#include <thirdparty/glm/gtc/matrix_transform.hpp>

namespace vray {

	const glm::mat4& CompTransform::getTransformMatrix() {
		if (dirty) {
			transformMatrix = glm::identity<glm::mat4>();
			transformMatrix = glm::translate(transformMatrix, position);
			transformMatrix *= glm::mat4_cast(rotation);
			transformMatrix = glm::scale(transformMatrix, scale);
			dirty = false;
		}
		return transformMatrix;
	}

	const glm::mat3& CompTransform::getNormalMatrix() const {
		//if (dirty) {
		//	normalMatrix = glm::mat3(glm::transpose(glm::inverse(transformMatrix)));
		//	dirty = false;
		//}
		return normalMatrix;
	}

	void CompTransform::setPosition(const glm::vec3& position) { 
		this->position = position;
		dirty = true;
		sync = false;
	}

	void CompTransform::setRotation(const glm::vec3& rotation) {
		this->rotation = glm::quat(rotation);
		dirty = true;
		sync = false;
	}

	void CompTransform::setRotation(const glm::quat& rotation) {
		this->rotation = rotation;
		dirty = true;
		sync = false;
	}

	void CompTransform::setScale(const glm::vec3& scale) { 
		this->scale = scale;
		dirty = true;
		sync = false;
	}

	void CompTransform::setNormalMatrix(const glm::mat3 normalMatrix) {
		this->normalMatrix = normalMatrix;
	}

	CompCamera::CompCamera(float _fov, float width, float height, float _near, float _far)
		:	fov(_fov), near(_near), far(_far),
			projectionMatrix(glm::perspectiveFov(glm::radians(_fov), width, height, _near, _far)),
			position(0.0f, 0.0f, 5.0f), rotation(0.0f), active(false), viewDirty(true) {

		viewMatrix = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void CompCamera::updateView() {
		glm::vec3 front;
		calculateFront(front);
		viewMatrix = glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f)); // !!!
	}

	void CompCamera::setViewMatrix(const glm::mat4& viewMatrix) {
		this->viewMatrix = viewMatrix;
	}
	void CompCamera::setPosition(const glm::vec3& position) {
		this->position = position;
		this->viewDirty = true;
	}
	void CompCamera::setRotation(const glm::vec3& rotation) {
		this->rotation = rotation;
		this->viewDirty = true;
	}

	void CompCamera::setProjectionBorders(float width, float height) {
		projectionMatrix = glm::perspectiveFov(glm::radians(fov), width, height, near, far);
	}

	const glm::mat4& CompCamera::getViewMatrix() {
		updateView();
		return viewMatrix;
	}

	void CompCamera::calculateFront(glm::vec3& front) {
		front.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		front.y = sin(glm::radians(rotation.y));
		front.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		front = glm::normalize(front);
	}

	void CompPointLightData::mergeColor(const glm::vec3& color) {
		la *= color;
		ld *= color;
		ls *= color;
	}

	void CompBillboardData::setPosition(const glm::vec3& position) {
		this->position = position;
		dirty = true;
	}

	void CompBillboardData::setSize(float size) {
		this->size = size;
		dirty = true;
	}

	void CompBillboardData::setTexture(Texture* texture) {
		this->texture = texture;
		textureChanged = true;
	}

	void CompSound::setPosition(const glm::vec3& position) {
		this->position = position; setPositionDirty(true);
	}

	void CompSound::setSourceId(int sourceId) {
		this->sourceId = sourceId;
	}

}