#include "vrpch.h"
#include "audio_exception.h"
#include "logservice.h"

namespace vray {

	const char* AudioException::what() const noexcept {
		VR_ENGINE_LOGERROR(message);
		return message.c_str();
	}

}