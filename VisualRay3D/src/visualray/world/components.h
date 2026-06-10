#pragma once
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtc/quaternion.hpp>
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
		bool active, viewDirty;

		/* Observing data */
		glm::mat4 viewMatrix, projectionMatrix;

		/* Camera body data */
		glm::vec3 position, rotation;
		float fov, near, far;

		void updateView();

	public:
		CompCamera(float fov, float width, float height, float near, float far);
		~CompCamera() {}

		void setViewMatrix(const glm::mat4& viewMatrix);
		void setProjectionMatrix(const glm::mat4& projectionMatrix) { this->projectionMatrix = projectionMatrix; }
		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);
		void setProjectionBorders(float width, float height);
		void setViewDirty(bool viewDirty) { this->viewDirty = viewDirty; }

		const glm::mat4& getViewMatrix();
		const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
		const glm::vec3& getPosition() const { return position; }
		const glm::vec3& getRotation() const { return rotation; }

		void calculateFront(glm::vec3& inFront);

		bool isActive() const { return active; }
		bool isViewDirty() const { return viewDirty; }
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

	struct VRAYLIB CompPointLight { bool dummy; };

	struct VRAYLIB CompPointLightData {
		glm::vec4 position;
		alignas(16) glm::vec3 la;
		alignas(16) glm::vec3 ld;
		alignas(16) glm::vec3 ls;

		void mergeColor(const glm::vec3& color);
	};

	struct VRAYLIB CompPointLightIndex {
		int index = -1;
		bool dirty = true;
		bool deleted = false;
	};

	struct VRAYLIB CompBillboard { bool dummy; };

	struct VRAYLIB CompBillboardData {
	private:
		glm::vec3 position;
		float size;
		Texture* texture;
		bool dirty, textureChanged;

	public:
		CompBillboardData() : position(1.0f), size(1.0f), texture(nullptr),
			dirty(true), textureChanged(false) {}

		const glm::vec3& getPosition() const { return position; }
		float getSize() const { return size; }
		Texture* getTexture() const { return texture; }
		bool isDirty() const { return dirty; }

		static constexpr size_t getOffsetOfPosition() { return offsetof(CompBillboardData, position); }
		static constexpr size_t getOffsetOfSize() { return offsetof(CompBillboardData, size); }

		void setPosition(const glm::vec3& position);
		void setSize(float size);
		void setTexture(Texture* texture);

		/* !!! Setting by this will DEFINITELY break an internal billboard rendering system !!! */
		void setDirty(bool dirty) { this->dirty = dirty; }

		/* !!! Setting by this will DEFINITELY break an internal billboard rendering system !!! */
		void setTextureChanged(bool textureChanged) { this->textureChanged = textureChanged; }
	};

	struct VRAYLIB CompBillboardIndex {
		size_t index = SIZE_MAX;
		bool deleted = false;
	};

	struct VRAYLIB CompSound {
	private:
		Sound* sound;
		float volume, pitch, maxDistance;
		int sourceId;
		glm::vec3 position;
		bool positionDirty;

	public:
		CompSound(Sound* _sound)
		:	sound(_sound),
			volume(1.0f),
			pitch(1.0f),
			maxDistance(1000.0f),
			position(0.0f), sourceId(-1),
			positionDirty(true) {}

		void setPositionDirty(bool dirty) { this->positionDirty = dirty; }
		void setPosition(const glm::vec3& position);
		void setPitch(float pitch) { this->pitch = pitch; }
		void setVolume(float volume) { this->volume = volume; }
		void setMaxDistance(float maxDistance) { this->maxDistance = maxDistance; }
		void setSourceId(int sourceId);

		Sound* getSound() const { return sound; }
		const glm::vec3& getPosition() const { return position; }
		float getVolume() const { return volume; }
		float getMaxDistance() const { return maxDistance; }
		float getPitch() const { return pitch; }
		int getSourceId() const { return sourceId; }
		bool isPositionDirty() const { return positionDirty; }
	};

	struct VRAYLIB CompSoundPlay {
		bool ignoreSourcePosition;
	};

	struct VRAYLIB CompSoundListener {
	private:
		glm::vec3 position;
		bool dirty;

	public:
		CompSoundListener(const glm::vec3& _position) : position(_position), dirty(true) {}

		void setDirty(bool dirty) { this->dirty = dirty; }
		void setPosition(const glm::vec3& position);

		bool isDirty() const { return dirty; }
		const glm::vec3& getPosition() const { return position; }
	};
}