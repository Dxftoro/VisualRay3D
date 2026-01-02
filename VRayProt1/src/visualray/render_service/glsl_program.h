/*

TODO: Uniform layouts and attribute layouts

*/

#pragma once
#include "vrpch.h"
#include "kernel.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glsl_uniform_buffer.h"

namespace vray {

	class GlslException : public std::runtime_error {
	private:
	public:
		enum ExceptionType { SHADER, PROGRAM };

		GlslException(const char* text)
			: std::runtime_error(text) {}
		GlslException(const std::string& text)
			: std::runtime_error(text) {}

		static void forceThrowFromLog(GLuint shaderId, ExceptionType type);
	};

	enum class ShaderType : unsigned int {
		VERTEX = 0x8B31,
		FRAGMENT = 0x8B30,
		GEOMETRY = 0x8DD9,
		TESS_CONTROL = 0x8E88,
		TESS_EVALUATION = 0x8E87,
		COMPUTE = 0x91B9
	};

	std::string loadFile(const std::string& filename);

	class GlslProgram;
	class GlslUniform {
	private:
		GLuint program;
		GLint location;
		std::string name;

	public:
		GlslUniform() : program(0), location(-1) {}
		GlslUniform(GlslProgram* program, const std::string& name);

		unsigned int VRAYLIB getProgram() const { return program; }
		int VRAYLIB getLocation() const { return location; }
		std::string getName() const { return name; }
	};

	class GlslProgram {
	private:
		GLuint handle;
		bool linked;
		GLuint lastUboBinding;
		std::unordered_map<std::string, GLuint> uboBindingTable;

	public:
		GlslProgram() : handle(0), linked(false), lastUboBinding(0) {}
		~GlslProgram();

		void VRAYLIB compileShader(const std::string& filename, ShaderType type);
		void VRAYLIB link();
		void VRAYLIB use();
		void VRAYLIB unuse();
		void VRAYLIB validate();

		int VRAYLIB getUniformLocation(const std::string& name);
		unsigned int VRAYLIB getHandle() const { return handle; }
		GlslUniform VRAYLIB getUniform(const std::string& name);

		GlslUniformBuffer VRAYLIB createUniformBuffer(const std::string& name, const void* data, size_t size);

		bool VRAYLIB isLinked() const { return linked; }

		//void bindFragDataLocation(GLuint location, const std::string& name);

		void VRAYLIB setUniform(const GlslUniform& uniform, const glm::mat2& matrix);
		void VRAYLIB setUniform(const GlslUniform& uniform, const glm::mat3& matrix);
		void VRAYLIB setUniform(const GlslUniform& uniform, const glm::mat4& matrix);
		void VRAYLIB setUniform(const GlslUniform& uniform, const glm::vec3& vec);
		void VRAYLIB setUniform(const GlslUniform& uniform, const glm::vec4& vec);

		void VRAYLIB printActiveUniforms() const;
	};

}