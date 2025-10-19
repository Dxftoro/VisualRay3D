#include "vrpch.h"
#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "renderer.h"
#include "event_service/game_events.h"
#include "event_service/mouse_events.h"

#include "input_codes.h"
#include "input_service/input_service.h"

#include "logservice.h"

namespace vray {

    Renderer::Renderer(Window* _currentWindow/*, VertexArray* _vertexArray*/)
        : currentWindow(_currentWindow)/*, vertexArray(_vertexArray)*/ {
        if (!gladLoadGL()) {
            throw std::runtime_error("Can't load OpenGL!");
        }

        camera = new FrustumCamera(90,
            currentWindow->getWidth(),
            currentWindow->getHeight(), 0.1f, 300.0f);

        try {
            program.compileShader("shaders\\triangle.vert", ShaderType::VERTEX);
            program.compileShader("shaders\\triangle.frag", ShaderType::FRAGMENT);
            program.link();
            program.validate();
        }
        catch (std::runtime_error exc) {
            VR_ENGINE_LOGERROR(exc.what());
            std::terminate();
        }

        modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_ALPHA_TEST);
        vertexArray->unbind();
    }

    Renderer::~Renderer() {
        delete vertexArray;
        if (camera) delete camera;
    }

    void Renderer::clear() {
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::update(float deltaTime) {
        glm::vec3 cameraPosition = camera->getPosition();
        static float moveSpeed = 10.0f;

        if (InputService::keyPressed(VR_KEY_W)) {
            cameraPosition.z -= moveSpeed * deltaTime;
            camera->setPosition(cameraPosition);
        }
        if (InputService::keyPressed(VR_KEY_S)) {
            cameraPosition.z += moveSpeed * deltaTime;
            camera->setPosition(cameraPosition);
        }
        if (InputService::keyPressed(VR_KEY_A)) {
            cameraPosition.x -= moveSpeed * deltaTime;
            camera->setPosition(cameraPosition);
        }
        if (InputService::keyPressed(VR_KEY_D)) {
            cameraPosition.x += moveSpeed * deltaTime;
            camera->setPosition(cameraPosition);
        }
        if (InputService::keyPressed(VR_KEY_Q)) {
            cameraPosition.y += moveSpeed * deltaTime;
            camera->setPosition(cameraPosition);
        }
        if (InputService::keyPressed(VR_KEY_E)) {
            cameraPosition.y -= moveSpeed * deltaTime;
            camera->setPosition(cameraPosition);
        }

        try {
            program.use();
            program.setUniform("projectionMatrix", camera->getProjectionMatrix());
            program.setUniform("viewMatrix", camera->getViewMatrix());
            program.setUniform("modelMatrix", modelMatrix);
            program.setUniform("normalMatrix", normalMatrix);
        }
        catch (std::runtime_error exc) {
            VR_ENGINE_LOGERROR(exc.what());
            return;
        }

        vertexArray->bind();
        vertexArray->drawElements(DrawMode::TRIANGLES);
        vertexArray->unbind();

        int errorCode;
        if ((errorCode = glGetError())) {
            VR_ENGINE_LOGERROR(std::to_string(errorCode));
        }
    }

    void Renderer::submit(const RenderRequest& request) {
        renderQueue.push(request);
    }

    void Renderer::flush() {
        while (!renderQueue.empty()) {
            renderQueue.front().complete();
            renderQueue.pop();
        }
    }

    void Renderer::onEvent(Event& evt) {
        static glm::vec2 mouseBase(currentWindow->getWidth() / 2, currentWindow->getHeight() / 2);
        static bool viewPicked = false;

        switch (evt.getType()) {
        case EventType::WINDOW_RESIZE: {
            WindowResizeEvent& resizeEvt = dynamic_cast<WindowResizeEvent&>(evt);
            glViewport(0, 0, resizeEvt.getWidth(), resizeEvt.getHeight());
            resizeEvt.dump();
            break;
        }
        case EventType::MOUSE_CLICK: {
            //MouseClickEvent clickEvt = dynamic_cast<MouseClickEvent&>(evt);
            viewPicked = true;
            break;
        }
        case EventType::MOUSE_MOVED: {
            if (viewPicked) {
                MouseMovedEvent& moveEvt = dynamic_cast<MouseMovedEvent&>(evt);

                glm::vec3 newRotation = camera->getRotation();
                newRotation.x += (moveEvt.getX() - mouseBase.x) / 5.0f;
                newRotation.y += (mouseBase.y - moveEvt.getY()) / 5.0f;

                if (newRotation.x < 0.0f) newRotation.x += 360.0f;
                if (newRotation.x > 360.0f) newRotation.x -= 360.0f;

                if (newRotation.y < -90.0f) newRotation.y = -90.0f;
                if (newRotation.y > 90.0f) newRotation.y = 90.0f;

                camera->setRotation(newRotation);
            }
            break;
        }
        default: break;
        }
    }

}