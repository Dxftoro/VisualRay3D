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

/*	Macro that deomposes glm::vec3 to x, y, z.
	Helpful when working with API that doesn't use vector structutes,
	such as ODE. */
#define VR_D(GLM_VEC) (GLM_VEC).x, (GLM_VEC).y, (GLM_VEC).z

	/*	Macro that deomposes glm::vec2 to x, y.
		Helpful when working with API that doesn't use vector structutes,
		such as ODE. */
#define VR_D2(GLM_VEC) (GLM_VEC).x, (GLM_VEC).y

#define VR_SASSERT(CONDITION, MESSAGE) static_assert(CONDITION, MESSAGE);

using GLuint	= unsigned int;
using GLenum	= unsigned int;
using GLint		= int;
using GLchar	= char;