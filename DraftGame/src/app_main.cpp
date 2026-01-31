#include <visualray.h>
#include <visualray/event_service/keyboard_events.h>
#include <visualray/layer_service/imgui_layer.h>

struct PlayerController {
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 forward = glm::vec3(0.0f), right = glm::vec3(0.0f);

	bool onGround = true;

	float maxSpeed = 10.0f;
	float acceleration = 2.0f;
	float airAcceleration = 0.5f;
	float friction = 0.5f;
	float stopSpeed = 3.0f;
	float jumpForce = 6.0f;
	float gravity = 16.8f;

	void calculateDirections(vray::CompCamera* camera) {
		float yaw = glm::radians(camera->getRotation().x);
		assert(!std::isnan(yaw) || "Yaw is nan!");

		forward = glm::vec3(cos(yaw), 0.0f, sin(yaw)); // !!!
		right = glm::vec3(-sin(yaw), 0.0f, cos(yaw));

		assert(!std::isnan(forward.x) && !std::isnan(forward.z));
		assert(!std::isnan(right.x) && !std::isnan(right.z));
	}

	void accelerate(const glm::vec3& wishDir, float wishSpeed, float deltaTime) {
		float currentSpeed = glm::dot(velocity, wishDir);
		float addSpeed = wishSpeed - currentSpeed;

		if (addSpeed <= 0.0f) return;

		float accelSpeed = acceleration * wishSpeed * deltaTime;
		if (accelSpeed > addSpeed) accelSpeed = addSpeed;

		assert(!std::isnan(accelSpeed) || "accelSpeed is nan!");
		velocity += wishDir * accelSpeed;
	}

	void accelerateAir(const glm::vec3& wishDir, float wishSpeed, float deltaTime) {
		float currentSpeed = glm::dot(velocity, wishDir);
		float addSpeed = wishSpeed - currentSpeed;

		if (addSpeed <= 0.0f) return;

		float accelSpeed = airAcceleration * wishSpeed * deltaTime;
		if (accelSpeed > addSpeed) accelSpeed = addSpeed;
		
		assert(!std::isnan(accelSpeed) || "accelSpeed is nan!");
		velocity += wishDir * accelSpeed;
	}

	void frict(float deltaTime) {
		float speed = glm::length(velocity);
		if (speed < 0.01f) {
			velocity.x = 0.0f;
			velocity.z = 0.0f;
			return;
		}

		float drop = 0.0f;
		if (onGround) {
			float control = speed < stopSpeed ? stopSpeed : speed;
			drop += control * friction * deltaTime;
		}

		float newSpeed = std::max(speed - drop, 0.0f) / speed;
		velocity *= newSpeed;
	}

};

class DraftGame : public vray::Game {
private:
	vray::EngineContext& engine;
	vray::GameContext& game;
	PlayerController pc;

	entt::entity player, teapot, plathform, someLight;
	vray::CompCamera* camera;

	float prevAngle, amplitude, frequency, timeAccumulator;
	
	inline void moveRotated(glm::vec3& position, const float angle, const float moveSpeed) {
		position.x += cos(angle) * moveSpeed * deltaTime();
		position.z += sin(angle) * moveSpeed * deltaTime();
	}

	inline void handleKeys() {
		vray::InputService& inputService = engine.inputService;
		glm::vec3 cameraPosition = camera->getPosition();
		static float moveSpeed = 10.0f;
		float currentAngle = glm::radians(camera->getRotation().x);

		vray::CompTransform& teapotTransform = game.world.get<vray::CompTransform>(teapot);
		glm::vec3 teapotPosition = teapotTransform.getPosition();

		if (inputService.keyPressed(VR_KEY_W)) {
			moveRotated(cameraPosition, currentAngle, moveSpeed);
		}
		if (inputService.keyPressed(VR_KEY_S)) {
			moveRotated(cameraPosition, currentAngle + glm::pi<float>(), moveSpeed);
		}
		if (inputService.keyPressed(VR_KEY_A)) {
			moveRotated(cameraPosition, currentAngle - glm::pi<float>() / 2, moveSpeed);
		}
		if (inputService.keyPressed(VR_KEY_D)) {
			moveRotated(cameraPosition, currentAngle + glm::pi<float>() / 2, moveSpeed);
		}
		if (inputService.keyPressed(VR_KEY_Q)) {
			cameraPosition.y += moveSpeed * deltaTime();
		}
		if (inputService.keyPressed(VR_KEY_E)) {
			cameraPosition.y -= moveSpeed * deltaTime();
		}

		if (inputService.keyPressed(VR_KEY_UP)) {
			teapotPosition.x += (moveSpeed - 1) * deltaTime();
			teapotTransform.setPosition(teapotPosition);
		}
		if (inputService.keyPressed(VR_KEY_DOWN)) {
			teapotPosition.x -= (moveSpeed - 1) * deltaTime();
			teapotTransform.setPosition(teapotPosition);
		}

		if (inputService.keyPressed(VR_KEY_LEFT)) {
			game.world.patch<vray::CompPointLight>(someLight, [](vray::CompPointLight& light) {
				light.position.x += (moveSpeed - 1) * deltaTime();
			});

			vray::CompTransform& transform = game.world.get<vray::CompTransform>(someLight);
			glm::vec3 position = transform.getPosition();
			position.x += (moveSpeed - 1) * deltaTime();
			transform.setPosition(position);
		}

		camera->setPosition(cameraPosition);
	}

	inline void handleKeysGrounded() {
		vray::InputService& inputService = engine.inputService;
		vray::CompTransform& transform = game.world.get<vray::CompTransform>(player);
		
		if (inputService.keyPressed(VR_KEY_R)) transform.setPosition({ 0.0f, 20.0f, 0.0f });
		
		glm::vec3 position = transform.getPosition();

		float forwardMove = 0.0f, sideMove = 0.0f;
		if (inputService.keyPressed(VR_KEY_W)) forwardMove += 1.0f;
		if (inputService.keyPressed(VR_KEY_S)) forwardMove -= 1.0f;
		if (inputService.keyPressed(VR_KEY_A)) sideMove -= 1.0f;
		if (inputService.keyPressed(VR_KEY_D)) sideMove += 1.0f;

		pc.calculateDirections(camera);
		glm::vec3 wishVel = (pc.forward * forwardMove) + (pc.right * sideMove);
		glm::vec3 wishDir = glm::vec3(0.0f);
		float wishSpeed = glm::length(wishVel);

		if (wishSpeed > 0.0001f) {
			wishDir = glm::normalize(wishVel);
		}
		
		if (wishSpeed > pc.maxSpeed) {
			wishVel *= pc.maxSpeed / wishSpeed;
			wishSpeed = pc.maxSpeed;
		}

		pc.frict(deltaTime());
		if (pc.onGround) pc.accelerate(wishDir, wishSpeed, deltaTime());
		else pc.accelerateAir(wishDir, wishSpeed, deltaTime());

		static float verticalVelocity = 0.0f;

		if (inputService.keyPressed(VR_KEY_SPACE) && pc.onGround) {
			verticalVelocity = pc.jumpForce;
			pc.onGround = false;
		}
		if (!pc.onGround) {
			verticalVelocity -= pc.gravity * deltaTime();
		}
		else verticalVelocity = 0.0f;

		pc.velocity.y = verticalVelocity * deltaTime();
		position += pc.velocity;

		transform.setPosition(position);
		camera->setPosition(position + glm::vec3(0.0f, 4.0f, 0.0f));
	}

	inline void detectGround() {
		const glm::vec3& position = game.world.get<vray::CompTransform>(player).getPosition();
		auto result = engine.physics->raycast(position + glm::vec3(0.0f, 2.0f, 0.0f), position);
		pc.onGround = (result != std::nullopt);
	}

	//inline void returnPlayerToCenter() {
	//	vray::CompTransform& transform = game.world.get<vray::CompTransform>(player);
	//	transform.setPosition({ 0.0f, 20.0f, 0.0f });
	//}

	inline void handleRotation(vray::Event& evt) {
		static glm::vec2 mouseBase(getWindow()->getWidth() * 0.5f, getWindow()->getHeight() * 0.5f);
		vray::InputService& inputService = engine.inputService;

		if (evt.getType() == vray::EventType::MOUSE_MOVED
			&& inputService.getCursorMode() == vray::InputService::CursorMode::DISABLED) {
			vray::MouseMovedEvent& moveEvt = dynamic_cast<vray::MouseMovedEvent&>(evt);
			double newX = moveEvt.getX();
			double newY = moveEvt.getY();

			glm::vec3 newRotation = camera->getRotation();
			newRotation.x += (newX - mouseBase.x) * 0.2f;
			newRotation.y += (mouseBase.y - newY) * 0.2f;

			mouseBase.x = newX;
			mouseBase.y = newY;

			if (newRotation.x < 0.0f) newRotation.x += 360.0f;
			if (newRotation.x > 360.0f) newRotation.x -= 360.0f;

			if (newRotation.y < -89.0f) newRotation.y = -89.0f;
			if (newRotation.y > 89.0f) newRotation.y = 89.0f;

			camera->setRotation(newRotation);
		}
	}

	inline void handleMouseUnlock(vray::Event& evt) {
		vray::InputService& inputService = engine.inputService;

		if (evt.getType() == vray::KEY_PRESSED) {
			vray::KeyPressedEvent keyEvt = dynamic_cast<vray::KeyPressedEvent&>(evt);
			if (keyEvt.getKeyCode() != VR_KEY_TAB) return;
			
			vray::InputService::CursorMode cursorMode = inputService.getCursorMode();
			if (cursorMode == vray::InputService::CursorMode::DISABLED)
				inputService.setCursorMode(vray::InputService::CursorMode::NORMAL);
			else
				inputService.setCursorMode(vray::InputService::CursorMode::DISABLED);
		}
	}

	inline void handleRaycast(vray::Event& evt) {
		vray::InputService& inputService = engine.inputService;
		
		if (evt.getType() == vray::MOUSE_CLICK) {
			vray::MouseClickEvent& mouseEvent = dynamic_cast<vray::MouseClickEvent&>(evt);
			if (mouseEvent.getMouseButtonCode() != VR_MOUSE_BUTTON_1) return;

			glm::vec3 cameraFront;
			camera->calculateFront(cameraFront);

			auto result = engine.physics->raycast(camera->getPosition(), cameraFront, 500);
			if (!result) return;
			
			engine.physicsDebugSystem->pushDebugLine(camera->getPosition(), result->hitPoint);
		}
	}

	void spawnCube(const glm::vec3& position) {
		entt::entity cube = game.world.create();
		
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

		game.world.emplace<vray::CompTransform>(cube, cubeTransform);
		game.world.emplace<vray::CompRenderable>(cube,
			vray::CompRenderable(game.meshes.get("ershik"), game.textures.get("default")));
		game.world.emplace<vray::CompHitbox>(cube, cubeHitbox);
	}

	void spawnTeapot(const glm::vec3& position) {
		entt::entity teapot = game.world.create();

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

		vray::CompRenderable teapotRenderable(game.meshes.get("teapot"), game.textures.get("stone_bricks"));
		game.world.emplace<vray::CompTransform>(teapot, teapotTransform);
		game.world.emplace<vray::CompHitbox>(teapot, teapotHitbox);
		game.world.emplace<vray::CompRenderable>(teapot, teapotRenderable);

		this->teapot = teapot;
	}

	//void spawnPlatformGrid(const glm::vec3& center, float cellSize, int size) {
	//	int cellCount = size * size;
	//	glm::vec3 cellPosition = {
	//		center.x - (cellSize)
	//	};

	//	for (int i = 0; i < cellCount; i++) {

	//	}
	//}

	entt::entity spawnLightMarker(const glm::vec3& position, const glm::vec3& color) {
		entt::entity lightMarker = game.world.create();

		vray::CompTransform transform;
		transform.setPosition(position);
		transform.setScale({ 0.5f, 0.5f, 0.5f });

		vray::CompRenderable renderable(game.meshes.get("cube"), game.textures.get("default"));
		vray::CompPointLight light = {
			.position = glm::vec4(position, 1.0f),
			.la = glm::vec3(0.03f),
			.ld = glm::vec3(0.6),
			.ls = glm::vec3(1.0)
		};

		light.mergeColor(color);

		game.world.emplace<vray::CompTransform>(lightMarker, transform);
		game.world.emplace<vray::CompRenderable>(lightMarker, renderable);
		game.world.emplace<vray::CompPointLight>(lightMarker, light);

		return lightMarker;
	}

public:
	DraftGame()
	:	Game(vray::WindowParams("Draft Game", 860, 482)),
		engine(getEngineContext()),
		game(getGameContext())
	{
		DraftGame::setFpsLimit(70);

		vray::InputService& inputService = engine.inputService;
		inputService.setCursorMode(vray::InputService::CursorMode::DISABLED);

		vray::Mesh* teapotMesh = game.meshes.load("models/teapot.obj", "teapot");
		vray::Mesh* cubeMesh = game.meshes.load("models/cube.obj", "cube");
		vray::Mesh* ershikMesh = game.meshes.load("models/Ershik.obj", "ershik");
		vray::Texture* stoneBricks = game.textures.load("textures/KAMEN.JPG", "stone_bricks");
		vray::Texture* defaultTexture = game.textures.load("textures/default.png", "default");

		prevAngle = 0.0f;
		amplitude = 2.0f;
		frequency = 0.5f;
		timeAccumulator = 0.0f;

		pc.onGround = true;
		player = game.world.create();
		plathform = game.world.create();

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

		vray::CompTransform playerTransform;
		playerTransform.setPosition({0.0f, 40.0f, 0.0f});

		game.world.emplace<vray::CompTransform>(player, playerTransform);
		camera = &game.world.emplace<vray::CompCamera>(player,
			engine.cameraSystem.createCamera(90.0f, 0.1f, 300.0f));
		camera->setPosition({ 0.0f, 30.0f, 0.0f });
		engine.cameraSystem.setActiveCamera(camera);

		game.world.emplace<vray::CompTransform>(plathform, plathformTransform);
		game.world.emplace<vray::CompHitbox>(plathform, plathformHitbox);
		game.world.emplace<vray::CompRenderable>(plathform, vray::CompRenderable(cubeMesh, defaultTexture));

		//for (int i = 0; i < 15; i++) {
		//	spawnCube({
		//		vray::frand(-5.0f, 5.0f),
		//		40.0,
		//		vray::frand(-5.0f, 5.0f)});
		//}

		//spawnTeapot({ 0.0f, 20.0f, 0.0f });
		//VR_LOGINFO("Teapot entity id is " + std::to_string((uint32_t)teapot));

		someLight = spawnLightMarker({ 3.0f, 15.0f, 0.0f }, { 0.2f, 2.0f, 2.0f });
		//entt::entity light1 = spawnLightMarker({ -3.0f, 15.0f, -10.0f }, { 1.0f, 0.3f, 0.4f });
	}
	~DraftGame() {}

	inline void update() override {
		detectGround();
		handleKeysGrounded();

		//vray::CompTransform& transform = world.get<vray::CompTransform>(teapot);

		//timeAccumulator += deltaTime();
		//transform.setPosition({0.0f, 0.0f, 
		//	amplitude * glm::sin(timeAccumulator * frequency)});
	}

	inline void onEvent(vray::Event& evt) { 
		handleRotation(evt);
		handleMouseUnlock(evt);
		handleRaycast(evt);
	}
};

VR_IMPLEMENT_GAME(DraftGame);