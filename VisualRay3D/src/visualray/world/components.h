#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "thirdparty/glm/glm.hpp"
#include "thirdparty/glm/gtc/quaternion.hpp"
#include "thirdparty/glm/gtx/string_cast.hpp"

#include "managers/resource.h"
#include "logservice.h"

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
		glm::vec3 size;

	public:
		CompTransform()
			: position(0.0f), rotation(glm::identity<glm::quat>()), size(1.0f),
			dirty(true), sync(false) {
		}

		CompTransform(
			const glm::vec3& _position,
			const glm::quat& _rotation,
			const glm::vec3& _size)
			: position(_position), rotation(_rotation), size(_size),
				dirty(true), sync(false) {}

		const glm::vec3& getPosition() const { return position; }
		const glm::quat& getRotation() const { return rotation; }
		const glm::vec3& getSize() const { return size; }
		bool isDirty() const { return dirty; };
		bool isSync() const { return sync; }

		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);
		void setRotation(const glm::quat& rotation);
		void setSize(const glm::vec3& size);
		void setDirty(bool dirty) { this->dirty = dirty; }
		void setSync(bool sync) { this->sync = sync; };
	};

	struct VRAYLIB CompTransformMatrices {
		glm::mat4 transform;
		glm::mat3 normal;
	};

	struct VRAYLIB CompRenderable {
		Mesh* mesh;
		Texture* texture;

		struct Material {
			alignas(16) glm::vec3 ka = glm::vec3(0.1f);
			alignas(16) glm::vec3 kd = glm::vec3(1.0f);
			alignas(16) glm::vec3 ks = glm::vec3(1.0f);
			float shininess = 32.0f;
		} material;

		CompRenderable(Mesh* _mesh, Texture* _texture)
			: mesh(_mesh), texture(_texture) {}
	};

	struct VRAYLIB CompCamera {
	private:
		bool active, viewDirty;
		glm::vec3 position, rotation;
		float fov, near, far;

		void updateView();

	public:
		CompCamera(float fov, float near, float far);
		~CompCamera() {}

		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);
		void setViewDirty(bool viewDirty) { this->viewDirty = viewDirty; }

		const glm::vec3& getPosition() const { return position; }
		const glm::vec3& getRotation() const { return rotation; }
		float getFov() const { return fov; }
		float getNear() const { return near; }
		float getFar() const { return far; }

		void calculateFront(glm::vec3& inFront) const;
		void calculateForward(glm::vec3& inForward) const;
		void calculateRight(glm::vec3& inRight) const;

		bool isActive() const { return active; }
		bool isViewDirty() const { return viewDirty; }
	};

	struct VRAYLIB CompHitbox {
		enum class ShapeType : short int { BOX, CAPSULE, SPHERE };
		enum class PhysType : short int { STATIC, DYNAMIC, KINEMATIC };

	private:
		ShapeType shapeType;
		PhysType physType;
		glm::vec3 size;
		float radius;
		float mass;
		bool dirty;

	public:
		CompHitbox(ShapeType _shapeType, PhysType _physType, const glm::vec3& _size, float _mass = 10.0f)
			: shapeType(_shapeType), physType(_physType),
			size(_size), mass(_mass), radius(1.0f), dirty(false) {
		}
		
		/* This one will construct a dynamic hitbox! */
		CompHitbox(ShapeType _shapeType, const glm::vec3& _size, float _mass = 10.0f)
			: shapeType(_shapeType), physType(PhysType::DYNAMIC),
			size(_size), mass(_mass), radius(1.0f), dirty(false) {
		}

		ShapeType getShapeType() const { return shapeType; }
		PhysType getPhysType() const { return physType; }
		const glm::vec3& getSize() const { return size; }
		float getRadius() const { return radius; }
		float getMass() const { return mass; }
		bool isDirty() const { return dirty; }

		void setSize(const glm::vec3& size);
		void setRadius(float radius);
		void setMass(float mass);
		void setDirty(bool dirty) { this->dirty = dirty; }
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
		float volume, pitch, refDistance, maxDistance;
		int sourceId;
		glm::vec3 position;
		bool positionDirty;

	public:
		CompSound(Sound* _sound)
		:	sound(_sound),
			volume(1.0f),
			pitch(1.0f),
			refDistance(1.0f),
			maxDistance(1000.0f),
			position(0.0f), sourceId(-1),
			positionDirty(true) {}

		void setPositionDirty(bool dirty) { this->positionDirty = dirty; }
		void setPosition(const glm::vec3& position);
		void setPitch(float pitch) { this->pitch = pitch; }
		void setVolume(float volume) { this->volume = volume; }
		void setRefDistance(float refDistance) { this->refDistance = refDistance; }
		void setMaxDistance(float maxDistance) { this->maxDistance = maxDistance; }
		void setSourceId(int sourceId);

		Sound* getSound() const { return sound; }
		const glm::vec3& getPosition() const { return position; }
		float getVolume() const { return volume; }

		/* The distance in meters at which the sound will be heard at it's 100% volume */
		float getRefDistance() const { return refDistance; }
		float getMaxDistance() const { return maxDistance; }
		float getPitch() const { return pitch; }
		int getSourceId() const { return sourceId; }
		bool isPositionDirty() const { return positionDirty; }
	};

	struct VRAYLIB CompSoundPlay {
		bool local;
	};

	struct VRAYLIB CompSoundListener {
		int gain;
	};
}