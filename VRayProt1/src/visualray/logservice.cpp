#include "vrpch.h"
#include "logger.h"
#include "logservice.h"

namespace vray {

	Logger* vray::LogService::engineLogger;
	Logger* vray::LogService::gameLogger;

	void vray::LogService::init() {
		engineLogger = new Logger(VR_LOGGER_PATTERN_DEFAULT, "VisualRay3D");
		gameLogger = new Logger(VR_LOGGER_PATTERN_DEFAULT, "Game");
	}

	void vray::LogService::destroy() {
		delete engineLogger;
		delete gameLogger;

		std::cout << "Log service destroyed!" << std::endl;
	}

	inline Logger* vray::LogService::getGameLogger() { return gameLogger; }
	inline Logger* vray::LogService::getEngineLogger() { return engineLogger; }

}