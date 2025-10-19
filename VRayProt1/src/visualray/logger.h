#pragma once
#pragma warning(disable: 4129)

#include "kernel.h"
#include "vrpch.h"

#define VR_TIMESTAMP_PATTERN_DEFAULT		"%Y-%m-%d %H:%M:%S"
#define VR_TIMESTAMP_STRBUFF_SIZE			80

#define VR_LOGGER_PATTERN_DEFAULT			"%s[%s] [%s/%s]: %s\033[0m\n"
#define VR_LOGGER_COLOR_RED					"\033[1;31m"
#define VR_LOGGER_COLOR_GREEN				"\033[0;32m"
#define VR_LOGGER_COLOR_YELLOW				"\033[0;33m"
#define VR_LOGGER_COLOR_WHITE				"\033[0;37m"
#define VR_LOGGER_COLOR_RESET				"\033[0m\n"

namespace vray {

	class Logger {
	private:
		std::string levelName;
		std::string pattern;
		mutable std::tm* timeinfo;

		inline void prepareTime(char* buffer, size_t buffSize) const;

	public:
		VRAYLIB Logger(const std::string& _pattern, const std::string& _levelName);
		VRAYLIB ~Logger();

		inline void VRAYLIB logInfo(const std::string& message) const;
		inline void VRAYLIB logWarn(const std::string& message) const;
		inline void VRAYLIB logError(const std::string& message) const;
	};

}