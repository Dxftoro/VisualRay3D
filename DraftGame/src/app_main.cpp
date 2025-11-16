#include <visualray.h>
#include <visualray/layer_service/imgui_layer.h>

class DraftGame : public vray::Game {
private:
	entt::entity player, teapot, cube;
	vray::CompCamera* camera;

	float amplitude, frequency, timeAccumulator;

	inline void handleKeys() {
		glm::vec3 cameraPosition = camera->getPosition();
		static float moveSpeed = 10.0f;

		if (vray::InputService::keyPressed(VR_KEY_W)) {
			cameraPosition.z -= moveSpeed * deltaTime();
			camera->setPosition(cameraPosition);
		}
		if (vray::InputService::keyPressed(VR_KEY_S)) {
			cameraPosition.z += moveSpeed * deltaTime();
			camera->setPosition(cameraPosition);
		}
		if (vray::InputService::keyPressed(VR_KEY_A)) {
			cameraPosition.x -= moveSpeed * deltaTime();
			camera->setPosition(cameraPosition);
		}
		if (vray::InputService::keyPressed(VR_KEY_D)) {
			cameraPosition.x += moveSpeed * deltaTime();
			camera->setPosition(cameraPosition);
		}
		if (vray::InputService::keyPressed(VR_KEY_Q)) {
			cameraPosition.y += moveSpeed * deltaTime();
			camera->setPosition(cameraPosition);
		}
		if (vray::InputService::keyPressed(VR_KEY_E)) {
			cameraPosition.y -= moveSpeed * deltaTime();
			camera->setPosition(cameraPosition);
		}
	}

	inline void handleRotation(vray::Event& evt) {
		static glm::vec2 mouseBase(getWindow()->getWidth() / 2, getWindow()->getHeight() / 2);

		if (evt.getType() == vray::EventType::MOUSE_MOVED) {
			//case EventType::MOUSE_CLICK: {
			//	//MouseClickEvent clickEvt = dynamic_cast<MouseClickEvent&>(evt);
			//	viewPicked = true;
			//	break;
			//}
			vray::MouseMovedEvent& moveEvt = dynamic_cast<vray::MouseMovedEvent&>(evt);

			glm::vec3 newRotation = camera->getRotation();
			newRotation.x += (moveEvt.getX() - mouseBase.x) / 5.0f;
			newRotation.y += (mouseBase.y - moveEvt.getY()) / 5.0f;

			if (newRotation.x < 0.0f) newRotation.x += 360.0f;
			if (newRotation.x > 360.0f) newRotation.x -= 360.0f;

			if (newRotation.y < -90.0f) newRotation.y = -90.0f;
			if (newRotation.y > 90.0f) newRotation.y = 90.0f;

			camera->setRotation(newRotation);
		}
	}

public:
	DraftGame() {
		DraftGame::setFpsLimit(70);
		vray::Mesh* teapotMesh = meshes.load("models/teapot.obj", "teapot");
		vray::Mesh* cubeMesh = meshes.load("models/cube.obj", "cube");
		vray::Texture* stoneBricks = textures.load("models/sponza/KAMEN.JPG", "stone_bricks");

		player = world.create();
		teapot = world.create();
		cube = world.create();

		vray::CompRenderable teapotRenderable(teapotMesh, textures.get("stone_bricks"));
		vray::CompTransform cubeTransform, teapotTransform;
		cubeTransform.setScale({ 5.0f, 5.0f, 5.0f });
		cubeTransform.setPosition({ -50.0f, 0.0f, -1.0f });

		teapotTransform.setRotation({ glm::radians(-90.0f), 0.0f, 0.0f});

		camera = &world.emplace<vray::CompCamera>(player, vray::CompCamera(90.0f,
			getWindow()->getWidth(),
			getWindow()->getHeight(), 0.1f, 300.0f));
		cameraSystem.setActiveCamera(camera);

		world.emplace<vray::CompTransform>(teapot, teapotTransform);
		world.emplace<vray::CompTransform>(cube, cubeTransform);

		world.emplace<vray::CompRenderable>(teapot, teapotRenderable);
		world.emplace<vray::CompRenderable>(cube,
			vray::CompRenderable(cubeMesh, textures.get("stone_bricks")));

		amplitude = 2.0f;
		frequency = 0.5f;
		timeAccumulator = 0.0f;
	}
	~DraftGame() {}

	inline void update() override {
		handleKeys();

		vray::CompTransform& transform = world.get<vray::CompTransform>(teapot);

		timeAccumulator += deltaTime();
		transform.setPosition({0.0f, 0.0f, 
			amplitude * glm::sin(timeAccumulator * frequency)});
	}

	inline void onEvent(vray::Event& evt) { handleRotation(evt); }
};

VR_IMPLEMENT_GAME(DraftGame);