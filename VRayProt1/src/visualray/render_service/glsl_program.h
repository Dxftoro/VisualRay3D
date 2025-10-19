#pragma once
#include "vrpch.h"
#include "kernel.h"

#include <glm/gtc/matrix_transform.hpp>

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

	class GlslProgram {
	private:
		GLuint handle;
		bool linked;

		int getUniformLocation(const std::string& name);

	public:
		GlslProgram() : handle(0), linked(false) {}
		~GlslProgram();

		void VRAYLIB compileShader(const std::string& filename, ShaderType type);
		void VRAYLIB link();
		void VRAYLIB use();
		void VRAYLIB unuse();
		void VRAYLIB validate();

		int VRAYLIB getHandle() const { return handle; }
		bool VRAYLIB isLinked() const { return linked; }

		//void bindFragDataLocation(GLuint location, const std::string& name);
		void VRAYLIB setUniform(const std::string& name, const glm::mat2& matrix);
		void VRAYLIB setUniform(const std::string& name, const glm::mat3& matrix);
		void VRAYLIB setUniform(const std::string& name, const glm::mat4& matrix);

		void VRAYLIB printActiveUniforms() const;
	};

}