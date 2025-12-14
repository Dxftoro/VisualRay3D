#pragma once

#include "vrpch.h"
#include "kernel.h"
#include "event_service/event.h"
#include "window_service/window.h"

#include <glm/glm.hpp>

//#include "vertex_array.h"
#include "glsl_program.h"
#include "frustum_camera.h"
#include "render_request.h"

#define VR_RENDERER_MAX_DEBUG_LINES     1000

namespace vray {

    struct CompCamera;
    class Renderer {
    private:
        std::queue<RenderRequest> renderQueue;
        CompCamera* camera;
        Window* currentWindow;
        GlslProgram program, debugProgram;
        
        GlslUniform uProjectionMatrix, uViewMatrix, 
                    uModelMatrix, uNormalMatrix;

        GlslUniform uDebugProjectionMatrix, uDebugViewMatrix;
        uint32_t debugVao, debugVbo, debugVertexCount;

        glm::mat4 modelMatrix;
        glm::mat3 normalMatrix;

        bool initialCamera;

    public:
        Renderer(Window* currentWindow);
        ~Renderer();

        void clear();
        void update(float deltaTime);
        void submit(RenderRequest&& query);
        inline void flush();

        void onEvent(Event& evt);

        void setCamera(CompCamera* camera);
        CompCamera*& getCamera() { return camera; }

        void updateDebugPrimitives(const std::vector<float>& vertexData,
            const std::vector<int>& elements);
    };

}