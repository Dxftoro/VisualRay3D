#pragma once
#include "logger.h"

namespace vray {

	class VRAYLIB LogService {
	private:
		static Logger* engineLogger;
		static Logger* gameLogger;

	public:
		static void init();
		static void destroy();

		static inline Logger* getEngineLogger();
		static inline Logger* getGameLogger();
	};

}

#define VR_LOGINFO(MESSAGE_STR)				vray::LogService::getGameLogger()->logInfo(MESSAGE_STR)
#define VR_LOGWARN(MESSAGE_STR)				vray::LogService::getGameLogger()->logWarn(MESSAGE_STR)
#define VR_LOGERROR(MESSAGE_STR)			vray::LogService::getGameLogger()->logError(MESSAGE_STR)

#define VR_ENGINE_LOGINFO(MESSAGE_STR)		vray::LogService::getEngineLogger()->logInfo(MESSAGE_STR)
#define VR_ENGINE_LOGWARN(MESSAGE_STR)		vray::LogService::getEngineLogger()->logWarn(MESSAGE_STR)
#define VR_ENGINE_LOGERROR(MESSAGE_STR)		vray::LogService::getEngineLogger()->logError(MESSAGE_STR)