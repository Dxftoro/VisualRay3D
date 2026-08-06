#define VR_ENTRY_POINT
#include <visualray.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <visualray/thirdparty/glm/gtx/string_cast.hpp>
#include <visualray/thirdparty/glm/gtc/matrix_transform.hpp>

struct PlayerController {
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 forward = glm::vec3(0.0f), right = glm::vec3(0.0f);
	float verticalVelocity = 0.0f;

	bool onGround = true;

	float maxSpeed = 11.4f;
	float acceleration = 8.0f;
	float airAcceleration = 1.0f;
	float friction = 3.0f;
	float stopSpeed = 1.57f;
	float jumpPower = 9.64f;
	float gravity = 28.6f;

	void calculateDirections(vray::CompCamera* camera) {
		float yaw = glm::radians(camera->getRotation().x);

		forward = glm::vec3(cos(yaw), 0.0f, sin(yaw)); // !!!
		right = glm::vec3(-sin(yaw), 0.0f, cos(yaw));
	}

	void accelerate(const glm::vec3& wishDir, float wishSpeed, float deltaTime) {
		float currentSpeed = glm::dot(velocity, wishDir);
		float addSpeed = wishSpeed - currentSpeed;

		if (addSpeed <= 0.0f) return;

		float accelSpeed = acceleration * wishSpeed * deltaTime;
		if (accelSpeed > addSpeed) accelSpeed = addSpeed;

		velocity += wishDir * accelSpeed;
	}

	void airAccelerate(const glm::vec3& wishDir, float wishSpeed, float deltaTime) {
		if (wishSpeed > 0.3f) wishSpeed = 0.3f;

		float currentSpeed = glm::dot(velocity, wishDir);
		float addSpeed = wishSpeed - currentSpeed;

		if (addSpeed <= 0.0f) return;

		float accelSpeed = airAcceleration * wishSpeed * deltaTime;
		if (accelSpeed > addSpeed) accelSpeed = addSpeed;

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
		velocity.x *= newSpeed;
		velocity.z *= newSpeed;
	}

	inline void jump() {
		verticalVelocity = jumpPower;
		onGround = false;
	}

	inline void fall(float deltaTime) {
		verticalVelocity -= gravity * deltaTime;
	}

};

class DraftGame : public vray::Game {
private:
	vray::EngineContext& engine;
	vray::GameContext& game;
	vray::Console* console;
	PlayerController pc;

	entt::entity player, teapot, light1, light2;
	vray::CompCamera* camera;

	vray::Texture* billboardTexture;

	float playerSpeed;

	inline void moveRotated(glm::vec3& position, const float angle, const float moveSpeed) {
		position.x += cos(angle) * moveSpeed * deltaTime();
		position.z += sin(angle) * moveSpeed * deltaTime();
	}

	inline void handleKeysGrounded() {
		vray::InputService& inputService = engine.inputService;
		vray::CompTransform& transform = game.world.get<vray::CompTransform>(player);

		if (inputService.keyPressed(VR_KEY_R)) {
			pc.velocity = glm::vec3(0.0f);
			pc.verticalVelocity = 0.0f;
			transform.setPosition({ 0.0f, 20.0f, 0.0f });
		}

		glm::vec3 position = transform.getPosition();

		float forwardMove = 0.0f, sideMove = 0.0f;
		if (!console->isOpened()) {
			if (inputService.keyPressed(VR_KEY_W)) forwardMove += 1.0f;
			if (inputService.keyPressed(VR_KEY_S)) forwardMove -= 1.0f;
			if (inputService.keyPressed(VR_KEY_A)) sideMove -= 1.0f;
			if (inputService.keyPressed(VR_KEY_D)) sideMove += 1.0f;
		}

		pc.calculateDirections(camera);
		glm::vec3 wishVel = (pc.forward * forwardMove) + (pc.right * sideMove); wishVel.y = 0.0f;
		glm::vec3 wishDir = glm::vec3(0.0f);
		float wishSpeed = glm::length(wishVel);

		if (wishSpeed > pc.maxSpeed) {
			wishVel *= pc.maxSpeed / wishSpeed;
			wishSpeed = pc.maxSpeed;
		}

		if (wishSpeed > 0.0001f) {
			wishDir = glm::normalize(wishVel);
		}

		pc.frict(deltaTime());
		if (pc.onGround) {
			pc.accelerate(wishDir, wishSpeed, deltaTime());
		}
		else {
			pc.airAccelerate(wishDir, wishSpeed, deltaTime());
		}

		if (inputService.keyPressed(VR_KEY_SPACE) && pc.onGround) {
			pc.jump();
		}
		if (!pc.onGround) {
			pc.fall(deltaTime());
		}
		else pc.verticalVelocity = 0.0f;

		playerSpeed = glm::length(pc.velocity);
		pc.velocity.y = pc.verticalVelocity * deltaTime();
		position += pc.velocity;

		transform.setPosition(position);
		camera->setPosition(position + glm::vec3(0.0f, 4.0f, 0.0f));

		if (inputService.keyPressed(VR_KEY_UP)) {
			auto& teapotTransform = game.world.get<vray::CompTransform>(teapot);
			teapotTransform.setSize(teapotTransform.getSize() * 1.05f);
			VR_LOGIMPORTANT(glm::to_string(teapotTransform.getSize()));
		}
		else if (inputService.keyPressed(VR_KEY_DOWN)) {
			auto& teapotTransform = game.world.get<vray::CompTransform>(teapot);
			teapotTransform.setSize(teapotTransform.getSize() * 0.95f);
			VR_LOGIMPORTANT(glm::to_string(teapotTransform.getSize()));
		}
	}

	inline void detectGround() {
		const glm::vec3& position = game.world.get<vray::CompTransform>(player).getPosition();
		auto result = engine.physics->raycastFront(position + glm::vec3(0.0f, 2.0f, 0.0f), position);
		pc.onGround = (result != std::nullopt);
	}

	inline void handleRotation(vray::Event& evt) {
		static glm::vec2 mouseBase(getWindow()->getWidth() * 0.5f, getWindow()->getHeight() * 0.5f);
		vray::InputService& inputService = engine.inputService;

		if (evt.getType() == vray::Eid(vray::events::MOUSE_MOVED)
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

		if (evt.getType() == vray::Eid(vray::events::KEY_PRESSED)) {
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

		if (evt.getType() == vray::Eid(vray::events::MOUSE_CLICK)) {
			vray::MouseClickEvent& mouseEvent = dynamic_cast<vray::MouseClickEvent&>(evt);
			if (mouseEvent.getMouseButtonCode() != VR_MOUSE_BUTTON_1) return;

			engine.audio->play(player);

			glm::vec3 cameraFront;
			camera->calculateFront(cameraFront);

			glm::vec3 start = camera->getPosition()/* + glm::vec3(0.0, -1.0f, 0.0f)*/;
			//engine.physicsDebugSystem->pushDebugLine(start, start + pc.forward * 5.0f);
			//engine.physicsDebugSystem->pushDebugLine(start, start + pc.right * 5.0f);

			auto result = engine.physics->raycastFront(
				start, start + cameraFront * 20.0f
			);
			
			if (!result) return;

			engine.physics->applyForceAtPoint(
				result->hitEntity,
				(cameraFront * 200.0f / deltaTime()) * -1.0f,
				result->hitPoint
			);

			engine.physicsDebugSystem->pushDebugLine(start, result->hitPoint);
			//glm::vec3 billboardPosition = result->hitPoint + result->hitNormal * 2.0f;
			//spawnBillboard(billboardPosition, 1.0f);
		}
	}

	inline void handleConsole(vray::Event& evt) {
		if (evt.getType() == vray::Eid(vray::events::KEY_PRESSED)) {
			vray::KeyPressedEvent& keyEvent = dynamic_cast<vray::KeyPressedEvent&>(evt);
			if (keyEvent.getKeyCode() == VR_KEY_ESCAPE) {
				console->setOpened(!console->isOpened());
			}
			else if (keyEvent.getKeyCode() == VR_KEY_U) {
				VR_LOGIMPORTANT("Changing teapot");
				auto& hitbox = game.world.get<vray::CompHitbox>(teapot);
				auto& size = hitbox.getSize();
				hitbox.setSize(size / 2.0f);
			}
		}
	}

	inline void handlePhysics(vray::Event& evt) {
		vray::EventDispatcher dispatcher(evt);
		dispatcher.fire<vray::CollisionEvent>([this](vray::CollisionEvent evt) -> bool {
			if (evt.getContactType() == vray::CollisionEvent::ContactType::START) {
				if (!game.world.try_get<CompCubeTag>(evt.getEntity1())) return true;

				auto& sound = game.world.get<vray::CompSound>(evt.getEntity1());
				sound.setPitch(vray::frand(0.8f, 1.2f));

				engine.audio->play(evt.getEntity1());
			}

			return true;
		});
	}

	struct CompCubeTag {
		bool dummy;
	};

	void spawnCube(const glm::vec3& position) {
		entt::entity cube = game.world.create();

		vray::CompTransform cubeTransform;
		//cubeTransform.setSize({ 1.0f, 1.0f, 1.0f });
		cubeTransform.setPosition(position);

		auto& renderable = game.world.emplace<vray::CompRenderable>(cube,
			vray::CompRenderable(game.meshes.get("cube"), game.textures.get("default")));

		vray::CompHitbox cubeHitbox(
			vray::CompHitbox::ShapeType::BOX,
			vray::CompHitbox::PhysType::DYNAMIC,
			cubeTransform.getSize(),
			12.0f
		);

		game.world.emplace<vray::CompTransform>(cube, cubeTransform);
		game.world.emplace<vray::CompHitbox>(cube, cubeHitbox);
		auto& sound = game.world.emplace<vray::CompSound>(cube, game.sounds.get("shoot"));
		sound.setMaxDistance(50.0f);
		sound.setRefDistance(10.0f);

		game.world.emplace<CompCubeTag>(cube, CompCubeTag(false));
	}

	void spawnTeapot(const glm::vec3& position) {
		entt::entity teapot = game.world.create();

		vray::CompRenderable teapotRenderable(game.meshes.get("teapot"), game.textures.get("stone_bricks"));
		teapotRenderable.material = {
			.ka = glm::vec3(0.1f),
			.kd = glm::vec3(1.0f),
			.ks = glm::vec3(0.1f),
			.shininess = 256.0f
		};

		game.world.emplace<vray::CompRenderable>(teapot, teapotRenderable);

		vray::CompTransform teapotTransform;
		teapotTransform.setPosition(position);
		teapotTransform.setRotation({ glm::radians(-90.0f), 0.0f, 0.0f });
		teapotTransform.setSize(teapotRenderable.mesh->getBaseSize() * 0.25f);
		//teapotTransform.setSize({ 0.25f, 0.25f, 0.25f });

		vray::CompHitbox teapotHitbox(
			vray::CompHitbox::ShapeType::BOX,
			vray::CompHitbox::PhysType::DYNAMIC,
			teapotTransform.getSize(),
			12.0f
		);

		game.world.emplace<vray::CompTransform>(teapot, teapotTransform);
		game.world.emplace<vray::CompHitbox>(teapot, teapotHitbox);
		this->teapot = teapot;
	}

	void spawnPlatform(const glm::vec3& position, float size) {
		entt::entity platform = game.world.create();

		vray::CompTransform transform;
		//transform.setPosition({ 0.0f, 5.0f, 0.0f });
		transform.setPosition(position);
		transform.setSize({ size, 1.0f, size });

		vray::CompHitbox hitbox(
			vray::CompHitbox::ShapeType::BOX,
			vray::CompHitbox::PhysType::STATIC,
			transform.getSize(),
			10.0f
		);

		game.world.emplace<vray::CompTransform>(platform, transform);
		game.world.emplace<vray::CompHitbox>(platform, hitbox);
		game.world.emplace<vray::CompRenderable>(platform,
			vray::CompRenderable(game.meshes.get("cube"), game.textures.get("default")));
	}

	void spawnPlatformGrid(const glm::vec3& center, float _cellSize, int size) {
		int cellCount = size * size;
		float cellSize = _cellSize;

		const glm::vec3 cellStartPosition = {
			center.x - (cellSize * (size / 2)) + cellSize * 0.5,
			center.y,
			center.z - (cellSize * (size / 2)) + cellSize * 0.5
		};

		VR_LOGINFO(std::to_string(cellStartPosition.x) + ", " + std::to_string(cellStartPosition.z));

		glm::vec3 cellPosition = cellStartPosition;
		spawnPlatform(cellPosition, cellSize);

		for (int i = 0; i < cellCount - 1; i++) {
			if (i > 0 && (i + 1) % size == 0) {
				cellPosition.x = cellStartPosition.x;
				cellPosition.z += cellSize;
			}
			else cellPosition.x += cellSize;

			spawnPlatform(cellPosition, cellSize);
		}
	}

	void spawnPlatformLine(const glm::vec3& start, float _cellSize, int size) {
		int cellCount = size * size;
		float cellSize = _cellSize + 2.0f;

		glm::vec3 cellPosition = start;

		for (int i = 0; i < cellCount; i++) {
			spawnPlatform(cellPosition, size);
			cellPosition.x += cellSize;
		}
	}

	entt::entity spawnLightMarker(const glm::vec3& position, const glm::vec3& color) {
		entt::entity lightMarker = game.world.create();

		vray::CompTransform transform;
		transform.setPosition(position);
		transform.setSize({ 0.5f, 0.5f, 0.5f });

		vray::CompRenderable renderable(game.meshes.get("cube"), game.textures.get("default"));

		game.world.emplace<vray::CompPointLight>(lightMarker);

		vray::CompPointLightData& lightData = game.world.get<vray::CompPointLightData>(lightMarker);
		lightData.position = glm::vec4(position, 1.0f);
		lightData.mergeColor(color);

		game.world.emplace<vray::CompTransform>(lightMarker, transform);
		game.world.emplace<vray::CompRenderable>(lightMarker, renderable);
		//game.world.emplace<vray::CompPointLight>(lightMarker, light);

		return lightMarker;
	}

	entt::entity spawnBillboard(const glm::vec3& position, float size) {
		entt::entity billboard = game.world.create();
		game.world.emplace<vray::CompBillboard>(billboard);

		vray::CompBillboardData& data = game.world.get<vray::CompBillboardData>(billboard);
		data.setTexture(billboardTexture);
		data.setPosition(position);
		data.setSize(size);

		return billboard;
	}

	entt::entity spawnPhysicsModel(const glm::vec3& position, const glm::vec3& scale, vray::Mesh* mesh) {
		entt::entity model = game.world.create();

		vray::CompTransform transform;
		transform.setPosition(position);
		transform.setSize(scale);

		vray::CompRenderable renderable(mesh, game.textures.get("default"));
		vray::CompHitbox hitbox(
			vray::CompHitbox::ShapeType::BOX,
			vray::CompHitbox::PhysType::DYNAMIC,
			scale,
			10.0f
		);

		game.world.emplace<vray::CompTransform>(model, transform);
		game.world.emplace<vray::CompRenderable>(model, renderable);
		game.world.emplace<vray::CompHitbox>(model, hitbox);

		return model;
	}

	void consoleTest(const std::vector<std::string>& args) {
		console->write("Arg count: " + STR(args.size()));
		for (const std::string& arg : args) {
			console->write(arg);
		}
	}

	void updateLight(entt::entity marker, bool reversed) {
		auto& transform = game.world.get<vray::CompTransform>(marker);
		const glm::vec3& position = transform.getPosition();

		static float delta = 0.0f;
		delta += deltaTime();
		transform.setPosition({
			23.0f * glm::cos(delta * 0.1f) * (reversed ? -1.0f : 1.0f),
			position.y,
			23.0f * glm::sin(delta * 0.1f) * (reversed ? -1.0f : 1.0f)
			});

		game.world.patch<vray::CompPointLight>(marker, [marker, this, &transform](vray::CompPointLight& light) {
			auto& data = game.world.get<vray::CompPointLightData>(marker);
			data.position.x = transform.getPosition().x;
			data.position.y = transform.getPosition().y;
			data.position.z = transform.getPosition().z;
		});
	}

public:
	DraftGame()
		: Game(vray::WindowParams("Draft Game", 1290, 723)),
		engine(getEngineContext()),
		game(getGameContext())
	{
		DraftGame::setFpsLimit(70);

		vray::InputService& inputService = engine.inputService;
		inputService.setCursorMode(vray::InputService::CursorMode::DISABLED);
		inputService.setRawMouseInputEnabled(true);

		vray::Mesh* teapotMesh = game.meshes.load("models/teapot.obj", "teapot");
		vray::Mesh* cubeMesh = game.meshes.load("models/cube.obj", "cube");
		vray::Mesh* ershikMesh = game.meshes.load("models/Ershik.obj", "ershik");
		vray::Texture* stoneBricks = game.textures.load("textures/KAMEN.JPG", "stone_bricks");
		vray::Texture* defaultTexture = game.textures.load("textures/default.png", "default");
		vray::Texture* ozuTexture = game.textures.load("textures/ozu.png", "ozu");
		vray::Sound* shootSound = game.sounds.load("sounds/click.ogg", "shoot");

		VR_LOGIMPORTANT("VisualRay3D version: " VR_VERSION);
		VR_LOGIMPORTANT("Sound channels: " + std::to_string(shootSound->getChannelCount()));

		billboardTexture = ozuTexture;

		setClearColor(glm::vec4(0.1f, 0.6f, 0.8f, 0.0f));

		pc.onGround = true;
		player = game.world.create();
		playerSpeed = 0.0f;

		console = engine.debugger->getConsole();
		console->addCommand("test", [this](const std::vector<std::string>& args) {
			consoleTest(args);
		}, "A test command.");

		console->addCommand("sett", [this](const std::vector<std::string>& args) {
			if (args.size() <= 1) {
				console->write("No such args!");
				return;
			}

			try {
				billboardTexture = game.textures.get(args[1]);
			}
			catch (std::exception exc) {
				console->write(exc.what());
			}
		});

		console->addCommand("aabbs", [this](const std::vector<std::string>& args) {
			if (args.size() <= 1) {
				console->write("No such args!");
				return;
			}

			bool enabled = (args[1] == "1") ? true : false;
			if (!engine.physicsDebugSystem) {
				return;
			}
			engine.physicsDebugSystem->setEnabled(enabled);
		});

		console->addCommand("load", [this](const std::vector<std::string>& args) {
			if (args.size() < 4) {
				console->write("No such args!");
				return;
			}

			glm::vec3 cameraFront, position;

			camera->calculateFront(cameraFront);
			auto result = engine.physics->raycastFront(
				camera->getPosition(),
				camera->getPosition() + cameraFront * 500.0f);

			if (!result) return;
			position = result->hitPoint + result->hitNormal * 5.0f;

			try {
				vray::Mesh* mesh = game.meshes.get("models/" + args[1]);
				glm::vec3 scale = { std::stof(args[2]), std::stof(args[3]), std::stof(args[4]) };
				spawnPhysicsModel(position, scale, mesh);
			}
			catch (std::runtime_error exc) {
				console->write(exc.what());
			}
		});

		engine.debugger->addVariable("Vert. vel.: %.3f", &pc.verticalVelocity);
		engine.debugger->addVariable("Speed: %.3f", &playerSpeed);
		engine.debugger->addVariable("Forward: (%.3f, %.3f, %.3f)", &pc.forward);
		engine.debugger->addVariable("Right: (%.3f, %.3f, %.3f)", &pc.right);

		vray::CompTransform platformTransform;

		platformTransform.setPosition({ 0.0f, 5.0f, 0.0f });
		platformTransform.setSize({ 10.0f, 1.0f, 10.0f });

		//vray::CompHitbox platformHitbox{
		//	.shapeType = vray::CompHitbox::ShapeType::BOX,
		//	.physType = vray::CompHitbox::PhysType::STATIC,
		//	.size = platformTransform.getScale(),
		//	.radius = 10,
		//	.mass = 10.0f
		//};

		vray::CompTransform playerTransform;
		playerTransform.setPosition({ 0.0f, 40.0f, 0.0f });

		game.world.emplace<vray::CompTransform>(player, playerTransform);
		camera = &game.world.emplace<vray::CompCamera>(player,
			engine.cameraSystem.createCamera(90.0f, 0.1f, 300.0f));
		camera->setPosition({ 0.0f, 30.0f, 0.0f });
		engine.cameraSystem.setActiveCamera(camera);

		engine.renderer->setTestTexture(ozuTexture);

		game.world.emplace<vray::CompSoundListener>(player, vray::CompSoundListener(1.0f));
		auto& sound = game.world.emplace<vray::CompSound>(player, shootSound);
		sound.setPosition({ 0.0f, 0.0f, 0.0f });
		sound.setMaxDistance(1000.0f);
		sound.setRefDistance(30.0f);

		for (int i = 0; i < 15; i++) {
			spawnCube({
				vray::frand(-5.0f, 5.0f),
				40.0,
				vray::frand(-5.0f, 5.0f) });
		}

		spawnTeapot({ 0.0f, 40.0f, 0.0f });

		//spawnPlatformLine({ 0.0f, 5.0f, 0.0f }, 10.0f, 7);
		spawnPlatformGrid({ 0.0f, 5.0f, 0.0f }, 15, 6);
		light1 = spawnLightMarker({ 20.0f, 15.0f, 0.0f }, { 0.2f, 2.0f, 2.0f });
		light2 = spawnLightMarker({ -10.0, 10.0, 10.0 }, { 0.3f, 1.0f, 0.1f });
	}
	~DraftGame() {}

	inline void update() override {
		//auto& teapotTransform = game.world.get<vray::CompTransform>(teapot);

		detectGround();
		handleKeysGrounded();

		updateLight(light1, true);
		updateLight(light2, false);

		game.world.view<CompCubeTag>().each([this](entt::entity entity, CompCubeTag& tag) {
			//if (engine.physics->testOverlap(entity, teapot)) {
			//	VR_LOGIMPORTANT("A cube is overlaped by the teapot!");
			//}

			auto& transform = game.world.get<vray::CompTransform>(entity);
			if (transform.isDirty()) {
				auto& sound = game.world.get<vray::CompSound>(entity);
				sound.setPosition(transform.getPosition());
			}
		});
	}

	inline void onEvent(vray::Event& evt) {
		if (!console->isOpened()) {
			handleRotation(evt);
			handleRaycast(evt);
		}

		handleMouseUnlock(evt);
		handleConsole(evt);
		handlePhysics(evt);
	}
};

VR_IMPLEMENT_GAME(DraftGame);