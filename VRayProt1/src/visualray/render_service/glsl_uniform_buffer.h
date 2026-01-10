#pragma once
#include "vrpch.h"
#include "kernel.h"

namespace vray {

	class GlslProgram;
	class GlslUniformBuffer {
	private:
		GLuint handle;
		GLuint binding;
		GLuint program;
		GLuint blockIndex;
		GLint size;
		std::string name;

	public:
		GlslUniformBuffer();
		GlslUniformBuffer(GlslProgram* program, const std::string& name, const void* data, size_t size);
		~GlslUniformBuffer();

		GlslUniformBuffer(const GlslUniformBuffer&) = delete;
		GlslUniformBuffer& operator=(const GlslUniformBuffer&) = delete;

		GlslUniformBuffer(GlslUniformBuffer&&) noexcept;
		GlslUniformBuffer& operator=(GlslUniformBuffer&&) noexcept;

		void bind();
		void unbind();

		/* Set data to the ALREADY BINDED buffer! */
		void setData(void* data, size_t size, size_t offset = 0);

		unsigned int VRAYLIB getHandle() { return handle; }
		unsigned int VRAYLIB getProgram() { return program; }
		unsigned int VRAYLIB getBinding() { return binding; }
		unsigned int VRAYLIB getBlockIndex() { return blockIndex; }
		int VRAYLIB getSize() { return size; }
		std::string getName() { return name; }

		void VRAYLIB setBinding(GLuint binding) { this->binding = binding; }

		void VRAYLIB printUniformData() const;
		void VRAYLIB invalidate();
	};

}