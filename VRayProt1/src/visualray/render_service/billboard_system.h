#pragma once
#include "kernel.h"
#include "../thirdparty/EnTT/entt.hpp"
#include "../world/components.h"

#include "glsl_program.h"

namespace vray {

	class BillboardSystem {
	private:
		GlslProgram program;
		CompCamera* camera;
		Texture* texture;

		GlslUniform uCameraPosition, uProjectionMatrix, uViewMatrix;
		GLuint vao, vbo;
		bool first;

		entt::registry& world;
		using BillboardGroup = decltype(world.group<CompBillboard>());
		BillboardGroup billboardGroup;

	private:
		static void onBillboardAdded(entt::registry& world, const entt::entity entity);
		static void onBillboardUpdated(entt::registry& world, const entt::entity entity);
		static void onBillboardRemoved(entt::registry& world, const entt::entity entity);

	public:
		BillboardSystem(entt::registry& world);
		~BillboardSystem();

		void init(CompCamera* camera) throw(GlslException);
		void setTexture(Texture* texture) { this->texture = texture; }
		void setCamera(CompCamera* camera) { this->camera = camera; }
		void update();
	};

}