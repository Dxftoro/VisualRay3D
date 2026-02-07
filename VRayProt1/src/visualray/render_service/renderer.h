#pragma once

#include "vrpch.h"
#include "kernel.h"
#include "event_service/event.h"
#include "window_service/window.h"
#include "world/components.h"
#include "../thirdparty/EnTT/entt.hpp"

#include <glm/glm.hpp>

//#include "vertex_array.h"
#include "glsl_program.h"
#include "frustum_camera.h"
#include "light_system.h"
#include "billboard_system.h"

#define VR_RENDERER_MAX_DEBUG_LINES     1000

namespace vray {

    /* Forward declarations */
    struct CompCamera;
    struct CompVisualMaterial;
    struct CompPointLight;
    class RenderRequest;
    class GlslUniformBuffer;

    class Renderer {
    private:
        std::queue<RenderRequest> renderQueue;
        CompCamera* camera;
        Window* currentWindow;
        GlslProgram program, debugProgram;

        GlslUniform uProjectionMatrix,
                    uViewMatrix, 
                    uModelMatrix,
                    uNormalMatrix;

        GlslUniform uDebugProjectionMatrix, uDebugViewMatrix;
        uint32_t debugVao, debugVbo, debugVertexCount;

        CompVisualMaterial material;
        CompPointLight light;

        GlslUniformBuffer uboLight, uboMaterial;
        LightSystem lightSystem;


        bool initialCamera;

    public:
        Renderer(Window* currentWindow, entt::registry& world);
        ~Renderer();

        void clear();
        void update(float deltaTime);
        void submit(RenderRequest&& query);
        inline void flush();

        void onEvent(Event& evt);

        void setCamera(CompCamera* camera);
        CompCamera* getCamera() const { return camera; }
        Window* getCurrentWindow() const { return currentWindow; }

        void updateDebugPrimitives(const std::vector<float>& vertexData,
            const std::vector<int>& elements);
    };

}