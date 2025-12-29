#include "vrpch.h"
#include "glsl_uniform_buffer.h"
#include "glsl_program.h"

#include <glad/glad.h>

namespace vray {

	GlslUniformBuffer::GlslUniformBuffer(GlslProgram* _program, const std::string& _name,
		GLuint _binding, size_t _size)
		: program(_program->getHandle()), name(_name), binding(_binding), size(_size) {

		glGenBuffers(1, &handle);
		glBindBuffer(GL_UNIFORM_BUFFER, handle);
			glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, binding, handle);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		blockIndex = glGetUniformBlockIndex(program, name.c_str());
		if (blockIndex != GL_INVALID_INDEX) glUniformBlockBinding(program, blockIndex, binding);
		else throw GlslException(name + " - invalid block index!");
	}

	GlslUniformBuffer::~GlslUniformBuffer() {
		glDeleteBuffers(1, &handle);
	}

	void GlslUniformBuffer::setData(void* data, size_t offset) {
		glBindBuffer(GL_UNIFORM_BUFFER, handle);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

}