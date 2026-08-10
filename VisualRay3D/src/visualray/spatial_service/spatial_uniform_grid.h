#pragma once

#include "../vrpch.h"
#include "volume.h"
#include "spatial.h"

namespace vray {

	struct CompUniformCell {
		Volume volume;
		size_t index;
	};

	class UniformGrid : public ISpatial {
	private:
		entt::dense_map<Volume, std::vector<entt::entity>, VolumeHash> cells;
		entt::registry& world;
		float cellSize, margin;

		Volume calculateVolume(const glm::vec3& point) const;
		Aabb calculateAabb(const glm::vec3& position, const glm::vec3& size) const;
		Aabb calculateFrustumAabb(const glm::vec3* corners) const;
		bool intersectsFrustum(const Frustum& frustum, const Aabb& aabb) const;

	public:
		UniformGrid(entt::registry& world, float cellSize, float margin);

		virtual void insert(entt::entity entity) override;
		virtual void remove(entt::entity entity) override;
		virtual void update(entt::entity entity) override;
		virtual void queryFrustum(const Frustum& frustum, FunctionRef<void(entt::entity)> callback) override;
		virtual void queryAabb(const Aabb& aabb, FunctionRef<void(entt::entity)> callback) override;
		virtual void querySphere(const glm::vec3& position, float radius, FunctionRef<void(entt::entity)> callback) override;
		
		/* If "last" set to true, will remove CompUniformCell component from the specified entity */
		void remove(entt::entity entity, bool last);
		void insert(entt::entity entity, const Volume& volume);

		float getCellSize() const { return cellSize; }
		float getMargin() const { return margin; }
	};

}