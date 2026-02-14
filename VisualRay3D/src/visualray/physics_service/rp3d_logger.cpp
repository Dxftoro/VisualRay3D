#include "vrpch.h"
#include "logservice.h"
#include "rp3d_logger.h"

namespace vray {

    void Rp3dLogger::log(Level level, const std::string& physicsWorldName,
        Category category, const std::string& message,
        const char* filename, int lineNumber) {

        std::string levelStr;
        switch (level) {
        case rp3d::Logger::Level::Error: levelStr = "RP3D_ERROR"; break;
        case rp3d::Logger::Level::Warning: levelStr = "RP3D_WARN"; break;
        case rp3d::Logger::Level::Information: levelStr = "RP3D_INFO"; break;
        default: levelStr = "RP3D_DEBUG"; break;
        }

        std::string logMessage = "[" + physicsWorldName + "] " + levelStr + ": " + message;
        VR_ENGINE_LOGINFO(logMessage);

        // Также логируй файл и строку для ошибок
        if (level == rp3d::Logger::Level::Error) {
            VR_ENGINE_LOGERROR("At " + std::string(filename) + ":" + std::to_string(lineNumber));
        }
    }
}