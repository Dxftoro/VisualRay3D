#pragma once

namespace vray {

	class CompRenderable;
	class CompTransform;

	class RenderRequest {
	public:
		CompRenderable* renderable;
		CompTransform* transform;
		unsigned int drawMode;

		RenderRequest(CompRenderable* _renderable, CompTransform* _transform, unsigned int _drawMode)
			: renderable(_renderable), transform(_transform), drawMode(_drawMode) {};
		//void complete() const;
	};

}