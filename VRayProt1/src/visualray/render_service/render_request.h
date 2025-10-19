#pragma once

namespace vray {

	class Mesh;
	class RenderRequest {
	private:
		const Mesh* mesh;
		unsigned int drawMode;

	public:
		RenderRequest(const Mesh* _mesh, unsigned int _drawMode)
			: mesh(_mesh), drawMode(_drawMode) {};
		void complete() const;
	};

}