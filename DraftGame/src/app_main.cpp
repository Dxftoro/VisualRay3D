#include <visualray.h>
#include <visualray/layer_service/imgui_layer.h>

class DraftGame : public vray::Game {
private:
	entt::entity player, teapot, plathform;
	vray::CompCamera* camera;

	float prevAngle, amplitude, frequency, timeAccumulator;
	
	inline void moveRotated(glm::vec3& cameraPosition, const float angle, const float moveSpeed) {
		cameraPosition.x += cos(angle) * moveSpeed * deltaTime();
		cameraPosition.z += sin(angle) * moveSpeed * deltaTime();
	}

	inline void handleKeys() {
		glm::vec3 cameraPosition = camera->getPosition();
		static float moveSpeed = 10.0f;
		float currentAngle = glm::radians(camera->getRotation().x);

		vray::CompTransform& teapotTransform = world.get<vray::CompTransform>(teapot);
		glm::vec3 teapotPosition = teapotTransform.getPosition();

		if (vray::InputService::keyPressed(VR_KEY_W)) {
			moveRotated(cameraPosition, currentAngle, moveSpeed);
		}
		if (vray::InputService::keyPressed(VR_KEY_S)) {
			moveRotated(cameraPosition, currentAngle + glm::pi<float>(), moveSpeed);
		}
		if (vray::InputService::keyPressed(VR_KEY_A)) {
			moveRotated(cameraPosition, currentAngle - glm::pi<float>() / 2, moveSpeed);
		}
		if (vray::InputService::keyPressed(VR_KEY_D)) {
			moveRotated(cameraPosition, currentAngle + glm::pi<float>() / 2, moveSpeed);
		}
		if (vray::InputService::keyPressed(VR_KEY_Q)) {
			cameraPosition.y += moveSpeed * deltaTime();
		}
		if (vray::InputService::keyPressed(VR_KEY_E)) {
			cameraPosition.y -= moveSpeed * deltaTime();
		}

		if (vray::InputService::keyPressed(VR_KEY_UP)) {
			teapotPosition.x += (moveSpeed - 1) * deltaTime();
			teapotTransform.setPosition(teapotPosition);
		}
		if (vray::InputService::keyPressed(VR_KEY_DOWN)) {
			teapotPosition.x -= (moveSpeed - 1) * deltaTime();
			teapotTransform.setPosition(teapotPosition);
		}

		camera->setPosition(cameraPosition);
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

			if (newRotation.y < -89.0f) newRotation.y = -89.0f;
			if (newRotation.y > 89.0f) newRotation.y = 89.0f;

			camera->setRotation(newRotation);
		}
	}

	void spawnCube(const glm::vec3& position) {
		entt::entity cube = world.create();
		
		vray::CompTransform cubeTransform;
		cubeTransform.setScale({ 1.0f, 1.0f, 1.0f });
		cubeTransform.setPosition(position);
		
		vray::CompHitbox cubeHitbox{
			.shapeType = vray::CompHitbox::ShapeType::BOX,
			.physType = vray::CompHitbox::PhysType::DYNAMIC,
			.size = cubeTransform.getScale() * 0.9f,
			.radius = 1,
			.mass = 12.0f
		};

		world.emplace<vray::CompTransform>(cube, cubeTransform);
		world.emplace<vray::CompRenderable>(cube,
			vray::CompRenderable(meshes.get("cube"), textures.get("default")));
		world.emplace<vray::CompHitbox>(cube, cubeHitbox);
	}

	void spawnTeapot(const glm::vec3& position) {
		entt::entity teapot = world.create();

		vray::CompTransform teapotTransform;
		teapotTransform.setPosition(position);
		teapotTransform.setRotation({ glm::radians(-90.0f), 0.0f, 0.0f });
		teapotTransform.setScale({ 0.25f, 0.25f, 0.25f });
		
		vray::CompHitbox teapotHitbox{
			.shapeType = vray::CompHitbox::ShapeType::BOX,
			.physType = vray::CompHitbox::PhysType::DYNAMIC,
			.size = glm::vec3(1.0f, 2.0f, 2.0f),
			.radius = 10,
			.mass = 12.0f
		};

		vray::CompRenderable teapotRenderable(/*nullptr*/ meshes.get("teapot"), textures.get("stone_bricks"));
		world.emplace<vray::CompTransform>(teapot, teapotTransform);
		world.emplace<vray::CompHitbox>(teapot, teapotHitbox);
		world.emplace<vray::CompRenderable>(teapot, teapotRenderable);

		this->teapot = teapot;
	}

public:
	DraftGame(int argc, char* argv[]) : Game(vray::WindowParams("Draft Game", 860, 482)) {
		DraftGame::setFpsLimit(70);
		vray::InputService::setMouseLocked(true);

		vray::Mesh* teapotMesh = meshes.load("models/teapot.obj", "teapot");
		vray::Mesh* cubeMesh = meshes.load("models/cube.obj", "cube");
		vray::Mesh* ershikMesh = meshes.load("models/Ershik.obj", "ershik");
		vray::Texture* stoneBricks = textures.load("textures/KAMEN.JPG", "stone_bricks");
		vray::Texture* defaultTexture = textures.load("textures/default.png", "default");

		prevAngle = 0.0f;
		amplitude = 2.0f;
		frequency = 0.5f;
		timeAccumulator = 0.0f;

		player = world.create();
		plathform = world.create();

		vray::CompTransform plathformTransform;
		
		plathformTransform.setPosition({ 0.0f, 5.0f, 0.0f });
		plathformTransform.setScale({ 20.0f, 1.0f, 20.0f });

		vray::CompHitbox plathformHitbox{
			.shapeType = vray::CompHitbox::ShapeType::BOX,
			.physType = vray::CompHitbox::PhysType::STATIC,
			.size = plathformTransform.getScale(),
			.radius = 10,
			.mass = 10.0f
		};

		camera = &world.emplace<vray::CompCamera>(player, vray::CompCamera(90.0f,
			getWindow()->getWidth(),
			getWindow()->getHeight(), 0.1f, 300.0f));
		camera->setPosition({ 0.0f, 30.0f, 0.0f });
		cameraSystem.setActiveCamera(camera);

		world.emplace<vray::CompTransform>(plathform, plathformTransform);
		world.emplace<vray::CompHitbox>(plathform, plathformHitbox);
		world.emplace<vray::CompRenderable>(plathform, vray::CompRenderable(cubeMesh, defaultTexture));

		for (int i = 0; i < 15; i++) {
			spawnCube({
				vray::frand(-5.0f, 5.0f),
				40.0,
				vray::frand(-5.0f, 5.0f)});
		}

		spawnTeapot({ 0.0f, 20.0f, 0.0f });
		VR_LOGINFO("Teapot entity id is " + std::to_string((uint32_t)teapot));
	}
	~DraftGame() {}

	inline void update() override {
		handleKeys();

		//vray::CompTransform& transform = world.get<vray::CompTransform>(teapot);

		//timeAccumulator += deltaTime();
		//transform.setPosition({0.0f, 0.0f, 
		//	amplitude * glm::sin(timeAccumulator * frequency)});
	}

	inline void onEvent(vray::Event& evt) { handleRotation(evt); }
};

VR_IMPLEMENT_GAME(DraftGame);