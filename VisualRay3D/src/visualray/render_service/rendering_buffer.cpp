#include "vrpch.h"
#include "rendering_buffer.h"
#include "glad/glad.h"

namespace vray {

	RenderingBuffer::RenderingBuffer() {
		glCreateBuffers(1, &handle);
	}

	RenderingBuffer::~RenderingBuffer() {
		glDeleteBuffers(1, &handle);
	}

	VertexBuffer::VertexBuffer(float* verticies, unsigned int size, const BufferLayout& _bufferLayout)
		: bufferLayout(_bufferLayout) {
		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
	}

	void VertexBuffer::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, handle);
	}

	void VertexBuffer::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	ElementBuffer::ElementBuffer(int* elements, unsigned int size) {
		count = size / sizeof(int);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, elements, GL_STATIC_DRAW);
	}

	void ElementBuffer::bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	}

	void ElementBuffer::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}