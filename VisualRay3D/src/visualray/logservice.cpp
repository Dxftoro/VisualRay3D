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
		engineLogger->logInfo("Destroying log service");
		delete engineLogger;
		delete gameLogger;
	}

	Logger* vray::LogService::getGameLogger() { return gameLogger; }
	Logger* vray::LogService::getEngineLogger() { return engineLogger; }

}