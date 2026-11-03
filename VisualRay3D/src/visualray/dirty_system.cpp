#include "vrpch.h"
#include "dirty_system.h"
#include "world/components.h"
#include "world/tags.h"

namespace vray {

	void DirtySystem::update() {
		auto renderableView = world.view<CompVisualyCooled, CompTransform, CompTransformMatrices, CompRenderable>();
		auto commonView = world.view<CompVisualyCooled, CompTransform>();

		renderableView.each([this](entt::entity entity, CompVisualyCooled& c,
			CompTransform& transform,
			CompTransformMatrices& matrices,
			CompRenderable& renderable) {
			if (!transform.isDirty()) return;

			matrices.transform = glm::identity<glm::mat4>();
			matrices.transform = glm::translate(matrices.transform, transform.getPosition());
			matrices.transform *= glm::mat4_cast(transform.getRotation());

			glm::vec3 scale = transform.getSize() / renderable.mesh->getBaseSize();

			matrices.transform = glm::scale(matrices.transform,
				transform.getSize() / renderable.mesh->getBaseSize());
		});

		commonView.each([this](entt::entity, CompVisualyCooled& c, CompTransform& transform) {
			transform.setDirty(false);
		});

		world.clear<CompVisualyCooled>();
	}

}