#include "vrpch.h"
#include "components.h"
#include "logservice.h"

#include <thirdparty/glm/gtc/matrix_transform.hpp>

namespace vray {

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

	void CompTransform::setSize(const glm::vec3& size) { 
		//VR_LOGIMPORTANT("Setting scale! " + glm::to_string(scale));
		this->size = size;
		dirty = true;
		sync = false;
	}

	CompCamera::CompCamera(float _fov, float _near, float _far)
	:	fov(_fov), near(_near), far(_far),
		position(0.0f, 0.0f, 5.0f),
		rotation(0.0f),
		active(false),
		viewDirty(true) {
		VR_ENGINE_LOGIMPORTANT("CAMERAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	}

	void CompCamera::setPosition(const glm::vec3& position) {
		this->position = position;
		this->viewDirty = true;
	}

	void CompCamera::setRotation(const glm::vec3& rotation) {
		this->rotation = rotation;
		this->viewDirty = true;
	}

	void CompCamera::calculateFront(glm::vec3& front) const {
		front.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		front.y = sin(glm::radians(rotation.y));
		front.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		front = glm::normalize(front);
	}

	void CompHitbox::setSize(const glm::vec3& size) {
		this->size = size;
		dirty = true;
	}

	void CompHitbox::setRadius(float radius) {
		this->radius = radius;
		dirty = true;
	}

	void CompHitbox::setMass(float mass) {
		this->mass = mass;
		dirty = true;
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