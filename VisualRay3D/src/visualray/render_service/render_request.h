#pragma once
#include "vrpch.h"

namespace vray {

	class CompRenderable;
	class CompTransform;
	class CompTransformMatrices;

	class RenderRequest {
	public:
		enum class Type : uint16_t {
			DRAW_MESH,
			DRAW_DEBUG_LINES
		};

		CompRenderable* renderable;
		CompTransform* transform;
		CompTransformMatrices* matrices;
		uint16_t drawMode;
		Type requestType;

		RenderRequest(CompRenderable* _renderable, CompTransform* _transform, CompTransformMatrices* _matrices,
			uint16_t _drawMode, Type _requestType = Type::DRAW_MESH)
			: renderable(_renderable), transform(_transform), matrices(_matrices),
			drawMode(_drawMode), requestType(_requestType) {};
	};

}