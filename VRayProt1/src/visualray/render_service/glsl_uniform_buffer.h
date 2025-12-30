#pragma once
#include "vrpch.h"
#include "kernel.h"

namespace vray {

	class GlslProgram;
	class GlslUniformBuffer {
	private:
		GLuint handle;
		GLuint program;
		GLuint binding;
		GLuint blockIndex;
		GLint size;
		std::string name;

	public:
		explicit GlslUniformBuffer() : handle(0), program(0), binding(0), blockIndex(0), size(0) {}
		explicit GlslUniformBuffer(GlslProgram* program, const std::string& name, size_t size);
		~GlslUniformBuffer();

		//GlslUniformBuffer(const GlslUniformBuffer& uniformBuffer) = delete;
		//GlslUniformBuffer& operator=(const GlslUniformBuffer& unifromBuffer) = delete;

		void setData(void* data, size_t size, size_t offset = 0);

		unsigned int VRAYLIB getProgram() { return program; }
		unsigned int VRAYLIB getBinding() { return binding; }
		unsigned int VRAYLIB getBlockIndex() { return blockIndex; }
		int VRAYLIB getSize() { return size; }
		std::string getName() { return name; }

		void VRAYLIB setBinding(GLuint binding) { this->binding = binding; }

		void VRAYLIB printUniformData();
	};

}