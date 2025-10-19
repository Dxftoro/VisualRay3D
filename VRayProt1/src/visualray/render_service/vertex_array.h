#pragma once
#include "vrpch.h"
#include "kernel.h"
#include "rendering_buffer.h"

namespace vray {

	enum class DrawMode : GLenum {
		POINTS			= 0x0000,
		LINES			= 0x0001,
		LINE_LOOP		= 0x0002,
		LINE_STRIP		= 0x0003,
		TRIANGLES		= 0x0004,
		TRIANGLE_STRIP	= 0x0005,
		TRIANGLE_FAN	= 0x0006,
		QUADS			= 0x0007
	};

	class VertexArray {
	private:
		GLuint handle;
		std::vector<VertexBuffer*> vertexBuffers;
		ElementBuffer* elementBuffer;

	public:
		VertexArray();
		~VertexArray();

		void bind();
		void unbind();

		void addVertexBuffer(VertexBuffer* vertexBuffer);
		void setElementBuffer(ElementBuffer* _elementBuffer) { elementBuffer = _elementBuffer; }

		void drawElements(DrawMode mode) const;
	};

}