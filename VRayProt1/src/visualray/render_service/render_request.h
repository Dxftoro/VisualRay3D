#pragma once

namespace vray {

	class Mesh;
	class RenderRequest {
	private:
		const Mesh* mesh;
		const void* transform;
		unsigned int drawMode;

	public:
		RenderRequest(const Mesh* _mesh, const void* _transform, unsigned int _drawMode)
			: mesh(_mesh), transform(_transform), drawMode(_drawMode) {};
		void complete() const;
	};

}