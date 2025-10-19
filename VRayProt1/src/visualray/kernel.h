#pragma once

#ifdef VR_PLATFORM_WIN
	#ifdef VR_INTERNAL
		#define VRAYLIB __declspec(dllexport)
	#else
		#define VRAYLIB __declspec(dllimport)
	#endif
#else
	#define VR_DLLEXPORT
	#define VR_DLLIMPORT
	#error VisualRay2D only is only for Windows!
#endif

#define SETBIT(BITPOS) 1 << BITPOS
#define STR(VALUE) std::to_string(VALUE)

using GLuint	= unsigned int;
using GLenum	= unsigned int;
using GLint		= int;
using GLchar	= char;