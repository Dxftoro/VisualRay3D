#pragma once

#include "vrpch.h"
#include "kernel.h"
#include "event_service/event.h"
#include "window_service/window.h"

#include <glm/glm.hpp>

#include "vertex_array.h"
#include "glsl_program.h"
#include "frustum_camera.h"
#include "render_request.h"

namespace vray {

    class Renderer {
    private:
        std::queue<RenderRequest> renderQueue;
        VertexArray* vertexArray;
        FrustumCamera* camera;
        Window* currentWindow;
        GlslProgram program;

        glm::mat4 modelMatrix;
        glm::mat3 normalMatrix;

    public:
        Renderer(Window* currentWindow/*, VertexArray* vertexArray*/);
        ~Renderer();

        void clear();
        void update(float deltaTime);
        void submit(const RenderRequest& query);
        void flush();

        void onEvent(Event& evt);

        FrustumCamera*& getCamera() { return camera; }
        void setVertexArray(VertexArray* vertexArray) { this->vertexArray = vertexArray; }
    };

}