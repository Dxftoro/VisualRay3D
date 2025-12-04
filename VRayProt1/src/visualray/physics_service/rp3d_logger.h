#pragma once
#include <reactphysics3d/reactphysics3d.h>

namespace vray {

    class Rp3dLogger : public rp3d::Logger {
    public:
        virtual void log(Level level, const std::string& physicsWorldName,
            Category category, const std::string& message,
            const char* filename, int lineNumber) override;
    };

}