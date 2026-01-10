#include "vrpch.h"
#include "glsl_uniform_buffer.h"
#include "glsl_program.h"

#include <glad/glad.h>
#include "../logservice.h"

namespace vray {

	GlslUniformBuffer::GlslUniformBuffer(GlslProgram* _program, const std::string& _name, const void* data, size_t _size)
		: program(_program->getHandle()), name(_name) {
		
		blockIndex = glGetUniformBlockIndex(program, name.c_str());
		if (blockIndex == GL_INVALID_INDEX) throw GlslException(name + " - invalid block index!");

		glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
		glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_BINDING, (GLint*)&binding);

		VR_ENGINE_LOGINFO("UBO " + name + " binding: "
			+ std::to_string(binding) + " index: " + std::to_string(blockIndex));

		if (size == _size) {
			VR_ENGINE_LOGINFO("Size of UBO " + name + ": " + std::to_string(size)
				+ ", expected: " + std::to_string(_size));
		}
		else {
			VR_ENGINE_LOGWARN("Size of UBO " + name + ": " + std::to_string(size)
				+ ", expected: " + std::to_string(_size));
		}

		glGenBuffers(1, &handle);
		glBindBuffer(GL_UNIFORM_BUFFER, handle);
			glBufferData(GL_UNIFORM_BUFFER, _size, data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, handle);
		glUniformBlockBinding(program, blockIndex, binding);
	}

	GlslUniformBuffer::GlslUniformBuffer() : handle(0), program(0), binding(0), blockIndex(0), size(0) {
	
	}

	GlslUniformBuffer::~GlslUniformBuffer() {
		glDeleteBuffers(1, &handle);
	}

	GlslUniformBuffer::GlslUniformBuffer(GlslUniformBuffer&& other) noexcept
	:	handle(other.handle),
		binding(other.binding),
		program(other.program),
		blockIndex(other.blockIndex),
		size(other.size),
		name(other.name)
	{
		other.invalidate();
	}

	GlslUniformBuffer& GlslUniformBuffer::operator=(GlslUniformBuffer&& other) noexcept {
		if (this == &other) return *this;

		handle = other.handle;
		binding = other.binding;
		program = other.program;
		blockIndex = other.blockIndex;
		size = other.size;
		name = std::move(other.name);

		other.invalidate();

		return *this;
	}

	void GlslUniformBuffer::bind() {
		glBindBuffer(GL_UNIFORM_BUFFER, handle);
	}

	void GlslUniformBuffer::unbind() {
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GlslUniformBuffer::setData(void* data, size_t size, size_t offset) {
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

    void GlslUniformBuffer::printUniformData() const {
        GLuint blockIndex = glGetUniformBlockIndex(program, name.c_str());
        if (blockIndex == GL_INVALID_INDEX) {
            std::cout << "Block " << name << " not found!" << std::endl;
            return;
        }

        GLint blockSize;
        glGetActiveUniformBlockiv(program, blockIndex,
            GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
        std::cout << name << " size: " << blockSize << std::endl;

        GLint numUniforms;
        glGetActiveUniformBlockiv(program, blockIndex,
            GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms);

        std::vector<GLint> indices(numUniforms);
        glGetActiveUniformBlockiv(program, blockIndex,
            GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices.data());

        for (int i = 0; i < numUniforms; i++) {
            char name[256];
            GLsizei length;
            GLint size;
            GLenum type;

            glGetActiveUniform(program, indices[i], sizeof(name),
                &length, &size, &type, name);

            GLint offset;
            glGetActiveUniformsiv(program, 1, (GLuint*)&indices[i],
                GL_UNIFORM_OFFSET, &offset);

            std::cout << "  " << name << ": offset=" << offset
                << ", type=" << type << std::endl;
        }
    }

	void GlslUniformBuffer::invalidate() {
		handle = 0;
		binding = 0;
		program = 0;
		blockIndex = GL_INVALID_INDEX;
	}

}