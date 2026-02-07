#include "vrpch.h"
#include "glsl_flexible_buffer.h"

#include "logservice.h"
#include <glad/glad.h>

namespace vray {

	void GlslBufferController::init(GLuint& handle, GLenum type, const void* data, size_t size, GlslUsage usage) {
		glGenBuffers(1, &handle);
		glBindBuffer(type, handle);
			glBufferData(type, size, data, (GLenum)usage);
		glBindBuffer(type, 0);
	}

	void GlslBufferController::resize(GLuint& handle, GLenum type, size_t size, GlslUsage usage) {
		GLuint newBuffer;
		glGenBuffers(1, &newBuffer);
		glBindBuffer(GL_COPY_READ_BUFFER, handle);
		glBindBuffer(GL_COPY_WRITE_BUFFER, newBuffer);

		glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, (GLenum)usage);

		if (handle) {
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
			glDeleteBuffers(1, &handle);
		}

		handle = newBuffer;
	}

	void GlslBufferController::free(GLuint& handle) {
		glDeleteBuffers(1, &handle);
	}

	void GlslBufferController::bind(GLuint handle, GLenum type) {
		glBindBuffer(type, handle);
	}

	void GlslBufferController::unbind(GLenum type) {
		glBindBuffer(type, 0);
	}

	void GlslBufferController::setSubData(GLuint handle, GLenum type, const void* data, size_t offset, size_t size) {
		glBindBuffer(type, handle);
			glBufferSubData(type, offset, size, data);
		glBindBuffer(type, 0);
	}

	void GlslBufferController::getSubData(GLuint handle, GLenum type, void* data, size_t offset, size_t size) {
		glBindBuffer(type, handle);
			glGetBufferSubData(type, offset, size, data);
		glBindBuffer(type, 0);
	}

}