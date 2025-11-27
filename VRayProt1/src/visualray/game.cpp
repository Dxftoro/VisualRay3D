#include "vrpch.h"
#include "game.h"

#include <GLFW/glfw3.h>
#include <reactphysics3d/reactphysics3d.h>

#include "event_service/game_events.h"
#include "layer_service/imgui_layer.h"
#include "input_service/input_service.h"
#include "managers/resource_loader.h"
#include "render_service/render_request.h"

#include "physics_service/rp3d_physics.h"

namespace vray {
	float Game::begTime = 0.0f;
	float Game::endTime = 0.0f;
	float Game::_deltaTime = 1.0f;
	int Game::fpsLimit = 30;

	void testPhysics() {
		//rp3d::PhysicsCommon physicsCommon;

		//// Create a physics world
		//rp3d::PhysicsWorld* world = physicsCommon.createPhysicsWorld();

		//// Create a rigid body in the world
		//rp3d::Vector3 position(0, 20, 0);
		//rp3d::Quaternion orientation = rp3d::Quaternion::identity();
		//rp3d::Transform transform(position, orientation);
		//rp3d::RigidBody* body = world->createRigidBody(transform);

		//const rp3d::decimal timeStep = 1.0f / 60.0f;

		//// Step the simulation a few steps
		//for (int i = 0; i < 20; i++) {

		//	world->update(timeStep);

		//	// Get the updated position of the body
		//	const rp3d::Transform& transform = body->getTransform();
		//	const rp3d::Vector3& position = transform.getPosition();

		//	// Display the position of the body
		//	std::cout << "Body Position: (" << position.x << ", " <<
		//		position.y << ", " << position.z << ")" << std::endl;
		//}
	}

	Game::Game() : running(false), cameraSystem(nullptr) {
		if (!glfwInit()) {
			VR_ENGINE_LOGERROR("Can't initialize GLFW!");
			throw std::runtime_error("Can't initialize GLFW!");
		}

		window = Window::create(WindowParams());
		window->setEventCallback(
			std::bind(&Game::onEventInternal, this, std::placeholders::_1)
		);

		InputService::init(window);
		pushOverlay(new ImGuiLayer(window));

		VR_ENGINE_LOGINFO("Trying to load mesh...");

		std::ifstream fin("models\\teapot.obj");
		if (!fin) {
			VR_ENGINE_LOGERROR("Can't open file!");
			throw std::runtime_error("Can't open file!");
		}

		visibleGroup = world.group<CompTransform>(entt::get<CompRenderable>);
		renderer = new Renderer(window);
		physics = new Rp3dPhysics(world);
		cameraSystem = CameraSystem(renderer);

		VR_ENGINE_LOGINFO((const char*)glGetString(GL_VENDOR));
		VR_ENGINE_LOGINFO((const char*)glGetString(GL_RENDERER));
		VR_ENGINE_LOGINFO((const char*)glGetString(GL_VERSION));

		testPhysics();
	}

	Game::~Game() {
		delete renderer;
		delete physics;

		// Let the memory leak lol
		//delete window;

		glfwTerminate();
		VR_ENGINE_LOGINFO("GLFW terminated.");
	}

	inline void Game::run() {
		running = true;
		VR_ENGINE_LOGINFO("Game application started running!");

		begTime = glfwGetTime();

		while (running) {
			auto frameBegin = std::chrono::high_resolution_clock::now();
			auto frameEnd = frameBegin + std::chrono::milliseconds(1000 / fpsLimit);

			endTime = glfwGetTime();
			_deltaTime = endTime - begTime;
			begTime = endTime;

			physics->update(_deltaTime);

			this->update();
			this->renderSubmit();

			renderer->clear();
			renderer->update(deltaTime());

			window->onUpdate();
			layerStack.update();

			GameTickEvent evt;
			window->swapBuffers();

			std::this_thread::sleep_until(frameEnd);
		}
	}

	inline void Game::stop() { running = false; }

	bool Game::onWindowClosing(WindowCloseEvent& evt) {
		running = false;
		window->setClosed(true);
		return true;
	}

	void Game::renderSubmit() {
		visibleGroup.each([this]
		(entt::entity entity, CompTransform& transform, CompRenderable& renderable) {
			RenderRequest request(&renderable, &transform, 4U);
			renderer->submit(std::move(request));
		});
	}

	void Game::onEventInternal(Event& evt) {
		EventDispatcher dispatcher(evt);
		dispatcher.fire<WindowCloseEvent>(
			std::bind(&Game::onWindowClosing, this, std::placeholders::_1)
		);

		InputService::tryLockMouse();
		onEvent(evt);
		renderer->onEvent(evt);
	}

	float Game::deltaTime() { return _deltaTime; }
}