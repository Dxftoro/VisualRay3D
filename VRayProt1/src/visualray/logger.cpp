#include "vrpch.h"
#include "logger.h"

namespace vray {

	vray::Logger::Logger(const std::string& _pattern, const std::string& _levelName)
		: pattern(_pattern), levelName(_levelName), timeinfo(nullptr) {}
	vray::Logger::~Logger() {}

	void vray::Logger::prepareTime(char* buffer, size_t buffSize) const {
		std::time_t seconds = time(0);
		timeinfo = std::localtime(&seconds);
		std::strftime(buffer, buffSize, VR_TIMESTAMP_PATTERN_DEFAULT, timeinfo);
	}

	void vray::Logger::logInfo(const std::string& message) const {
		char buffer[VR_TIMESTAMP_STRBUFF_SIZE];
		prepareTime(buffer, sizeof(buffer));

		std::printf(pattern.c_str(), VR_LOGGER_COLOR_WHITE, buffer,
			levelName.c_str(), "INFO", message.c_str());
	}

	void vray::Logger::logWarn(const std::string& message) const {
		char buffer[VR_TIMESTAMP_STRBUFF_SIZE];
		prepareTime(buffer, sizeof(buffer));

		std::printf(pattern.c_str(), VR_LOGGER_COLOR_YELLOW, buffer,
			levelName.c_str(), "WARN", message.c_str());
	}

	void vray::Logger::logError(const std::string& message) const {
		char buffer[VR_TIMESTAMP_STRBUFF_SIZE];
		prepareTime(buffer, sizeof(buffer));

		std::fprintf(stderr, pattern.c_str(), VR_LOGGER_COLOR_RED, buffer,
			levelName.c_str(), "ERROR", message.c_str());
	}

	void vray::Logger::logImportant(const std::string& message) const {
		char buffer[VR_TIMESTAMP_STRBUFF_SIZE];
		prepareTime(buffer, sizeof(buffer));

		std::printf(pattern.c_str(), VR_LOGGER_COLOR_BLUE, buffer,
			levelName.c_str(), "IMPORTANT", message.c_str());
	}

	/* ---------------------------------------------- */

}