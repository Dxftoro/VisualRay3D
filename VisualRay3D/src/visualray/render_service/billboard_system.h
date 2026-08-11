#pragma once
#include "../thirdparty/EnTT/entt.hpp"
#include "../world/components.h"

#include "glsl_program.h"
#include "glsl_flexible_buffer.h"

#define VR_RENDERER_BILLBOARD_INSIZE	50
#define VR_RENDERER_BILLBOARD_EXTENT	100
#define VR_RENDERER_BILLBOARD_NEW		SIZE_MAX

namespace vray {

	class CameraSystem;

	/*	FIXME: GlslFlexibleBuffer resizing copies it's data to the HOST memory
		which makes a recently spawned billboards to not render at all!

		PSOL: replace GlslFlexibleBuffer with a coil buffer
	*/
	using BillboardVboType = GlslFlexibleBuffer<0x8892, CompBillboardData>;

	class BillboardVbo : public BillboardVboType {
	private:
		/*	The chache helps to reduce the count of driver calls
			which copy/move some data to the GPU memory. */
		CompBillboardData cache;

	public:
		BillboardVbo() : BillboardVboType(GlslUsage::DYNAMIC_DRAW, VR_RENDERER_BILLBOARD_EXTENT) {}

		void remove(size_t index);
		void add(const CompBillboardData& billboard);
		void update(size_t index, const CompBillboardData& billboard);
	};

	struct BillboardBatch {
		GLuint vao;
		BillboardVbo vbo;
		BillboardBatch() : vao(0) {}
	};

	using BatchTableIterator = std::unordered_map<Texture*, BillboardBatch>::iterator;

	class BillboardSystem {
	private:
		CameraSystem* cameraSystem;
		Texture* texture;

		GlslProgram program;
		GlslUniform uCameraPosition, uProjectionMatrix, uViewMatrix;
		
		std::unordered_map<Texture*, BillboardBatch> batchTable;

		entt::registry& world;
		using BillboardGroup = decltype(world.group<CompBillboardIndex>(entt::get<CompBillboardData>));
		BillboardGroup billboardGroup;

		entt::entity lastExisting;
		bool somethingDeleted;

	private:
		//static float vertexData[];

		static void onBillboardAdded(entt::registry& world, const entt::entity entity);
		void onBillboardRemoved(entt::registry& world, const entt::entity entity);

	public:
		BillboardSystem(entt::registry& world);
		~BillboardSystem();

		void init(CameraSystem* camera) throw(GlslException);
		void setTexture(Texture* texture) { this->texture = texture; }
		void update();
		
		BatchTableIterator createBatch(Texture* texture);
	};

}