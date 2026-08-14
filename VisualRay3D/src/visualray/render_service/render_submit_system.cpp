#include "vrpch.h"
#include "render_submit_system.h"
#include "renderer.h"
#include "render_request.h"
#include "../world/components.h"

#include <thirdparty/glm/gtc/matrix_access.hpp>

namespace vray {

	RenderSubmitSystem::RenderSubmitSystem(Renderer* _renderer, entt::registry& _world)
	:	renderer(_renderer), world(_world) {

		frustumCache.position = glm::vec3(0.0f);
		frustumCache.far = 300.0f;

		for (int i = 0; i < 6; i++) frustumCache.planes[i] = glm::vec4(0.0f);
		for (int i = 0; i < 8; i++) frustumCache.corners[i] = glm::vec3(0.0f);
	}

	void RenderSubmitSystem::update(SpatialSystem& spatialSystem, CameraSystem& cameraSystem) {
		const glm::mat4& projectionView = cameraSystem.getProjectionViewCache();
		if (cameraSystem.getActiveCamera()->isViewDirty()) {
			updateFrustum(cameraSystem, projectionView);
		}

		//auto visibleView = world.view<CompTransform, CompRenderable, CompTransformMatrices>();
		//visibleView.each([this]
		//(entt::entity entity, CompTransform& transform, CompRenderable& renderable, CompTransformMatrices& matrices) {
		//	RenderRequest request(&renderable, &transform, &matrices, 4U);
		//	renderer->submit(std::move(request));
		//});

		spatialSystem.queryFrustum(frustumCache, [this](entt::entity entity, CompTransform& transform) {
			auto [renderable, matrices] = world.try_get<CompRenderable, CompTransformMatrices>(entity);

			if (renderable) {
				RenderRequest request(renderable, &transform, matrices, 4U);
				renderer->submit(std::move(request));
			}
		});
	}

	void RenderSubmitSystem::updateFrustum(CameraSystem& cameraSystem, const glm::mat4& projectionView) {
		frustumCache.position = cameraSystem.getActiveCamera()->getPosition();
		frustumCache.far = cameraSystem.getActiveCamera()->getFar();

		frustumCache.planes[0] = glm::row(projectionView, 3) + glm::row(projectionView, 0);
		frustumCache.planes[1] = glm::row(projectionView, 3) - glm::row(projectionView, 0);
		frustumCache.planes[2] = glm::row(projectionView, 3) + glm::row(projectionView, 1);
		frustumCache.planes[3] = glm::row(projectionView, 3) - glm::row(projectionView, 1);
		frustumCache.planes[4] = glm::row(projectionView, 3) + glm::row(projectionView, 2);
		frustumCache.planes[5] = glm::row(projectionView, 3) - glm::row(projectionView, 2);

		for (int i = 0; i < 6; i++) {
			frustumCache.planes[i] /= glm::length(glm::vec3(frustumCache.planes[i]));
		}

		glm::mat4 invProjectionView = glm::inverse(projectionView);
		glm::vec4 ndcCorners[8] = {
			{-1, -1, -1, 1}, {1, -1, -1, 1}, {1, 1, -1, 1}, {-1, 1, -1, 1},
			{-1, -1,  1, 1}, {1, -1,  1, 1}, {1, 1,  1, 1}, {-1, 1,  1, 1}
		};

		for (int i = 0; i < 8; i++) {
			glm::vec4 worldPosition = invProjectionView * ndcCorners[i];
			frustumCache.corners[i] = glm::vec3(worldPosition) / worldPosition.w;
		}
	}

}