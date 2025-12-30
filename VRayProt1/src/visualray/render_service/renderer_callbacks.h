#pragma once
#include "kernel.h"
#include "vrpch.h"
#include "logservice.h"
#include <glad/glad.h>

namespace vray {

	std::string sourceEnumToString(GLenum source) {
		switch (source) {
		case GL_DEBUG_SOURCE_API:			return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW_SYSTEM";
		case GL_DEBUG_SOURCE_THIRD_PARTY:	return "THIRD_PARTY";
		case GL_DEBUG_SOURCE_APPLICATION:	return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER:			return "OTHER";
		default:							return "UNKNOWN";
		}
	}

	std::string typeEnumToString(GLenum type) {
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:				return "TYPE_ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY:			return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE:			return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER:				return "MARKER";
		case GL_DEBUG_TYPE_PUSH_GROUP:			return "PUSH_GROUP";
		case GL_DEBUG_TYPE_POP_GROUP:			return "POP_GROUP";
		case GL_DEBUG_TYPE_OTHER:				return "OTHER";
		default:								return "UNKNOWN";
		}
	}

	std::string severityEnumToString(GLenum severity) {
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:			return "HIGH";
		case GL_DEBUG_SEVERITY_MEDIUM:			return "MEDIUM";
		case GL_DEBUG_SEVERITY_LOW:				return "LOW";
		case GL_DEBUG_SEVERITY_NOTIFICATION:	return "NOTIFICATION";
		default:								return "UNKNOWN";
		}
	}

	void rendererDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar* message, const void* param) {

		std::string sourceStr = sourceEnumToString(source);
		std::string typeStr = typeEnumToString(type);
		std::string severityStr = severityEnumToString(severity);
		std::string result = std::format("OpenGL: {0}:{1}[{2}]({3}): {4}",
			sourceStr, typeStr, severityStr, id, message);

		if (severity == GL_DEBUG_SEVERITY_HIGH)
			VR_ENGINE_LOGERROR(result);
		else if (severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_LOW)
			VR_ENGINE_LOGWARN(result);
		else VR_ENGINE_LOGINFO(result);
	}

}