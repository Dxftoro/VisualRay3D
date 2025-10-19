#include "vrpch.h"
#include "logservice.h"
#include "vertex_array.h"

#include <glad/glad.h>

namespace vray {

	VertexArray::VertexArray() : elementBuffer(nullptr) {
		glGenVertexArrays(1, &handle);
	}

	VertexArray::~VertexArray() {
		unbind();
		for (VertexBuffer* buffer : vertexBuffers) delete buffer;
		glDeleteVertexArrays(1, &handle);
	}

	void VertexArray::bind() { glBindVertexArray(handle); }
	void VertexArray::unbind() { glBindVertexArray(0); }

	void VertexArray::addVertexBuffer(VertexBuffer* vertexBuffer) {
		BufferLayout layout = vertexBuffer->getBufferLayout();

		for (GLuint i = 0; i < layout.size(); i++) {
			glEnableVertexAttribArray(i);

			GLuint objectSize = layout[i].getTypeSize() * layout[i].getCount();

			VR_ENGINE_LOGINFO(layout[i].getName() + ": object stride: " + std::to_string(layout.getStride()));

			glVertexAttribPointer(i,
				layout[i].getCount(),
				layout[i].getGlType(),
				layout[i].isNormalized() ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)layout[i].getOffset());
		}

		vertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::drawElements(DrawMode mode) const {
		glDrawElements((GLenum)mode, elementBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}