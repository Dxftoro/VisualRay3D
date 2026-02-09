#pragma once
#include "kernel.h"
#include "../thirdparty/EnTT/entt.hpp"
#include "../world/components.h"

#include "glsl_program.h"
#include "glsl_flexible_buffer.h"

#define VR_RENDERER_BILLBOARD_INSIZE	50
#define VR_RENDERER_BILLBOARD_EXTENT	100
#define VR_RENDERER_BILLBOARD_NEW		SIZE_MAX

namespace vray {

	using BillboardVboType = GlslFlexibleBuffer<0x8892, CompBillboard>;

	class BillboardVbo : public BillboardVboType {
	public:
		BillboardVbo() : BillboardVboType(GlslUsage::STREAM_DRAW, VR_RENDERER_BILLBOARD_EXTENT) {}
	};

	struct BillboardBatch {
		GLuint vao;
		BillboardVbo vbo;
		BillboardBatch() : vao(0) {}
	};

	using BatchTableIterator = std::unordered_map<Texture*, BillboardBatch>::iterator;

	class BillboardSystem {
	private:
		CompCamera* camera;
		Texture* texture;

		GlslProgram program;
		GlslUniform uCameraPosition, uProjectionMatrix, uViewMatrix;
		
		std::unordered_map<Texture*, BillboardBatch> batchTable;

		entt::registry& world;
		using BillboardGroup = decltype(world.group<CompBillboardIndex>(entt::get<CompBillboard>));
		BillboardGroup billboardGroup;

	private:
		static float vertexData[];

		static void onBillboardAdded(entt::registry& world, const entt::entity entity);
		static void onBillboardRemoved(entt::registry& world, const entt::entity entity);

	public:
		BillboardSystem(entt::registry& world);
		~BillboardSystem();

		void init(CompCamera* camera) throw(GlslException);
		void setTexture(Texture* texture) { this->texture = texture; }
		void setCamera(CompCamera* camera) { this->camera = camera; }
		void update();
		
		BatchTableIterator createBatch(Texture* texture);
	};

}