#define VR_ENTRY_POINT
#include <visualray.h>

#include "spatial_test.h"
#include "spawner.h"
#include "controllers/player_controller.h"
#include "components.h"

SpatialTest::SpatialTest()
:	Game(vray::WindowParams("Spatial test", 1290, 723)),
	engine(getEngineContext()),
	game(getGameContext()) {

	SpatialTest::setFpsLimit(70);
	console = engine.debugger->getConsole();

	try {
		loadAssets();
		setupCommands();
	}
	catch (std::exception& exc) {
		VR_ENGINE_LOGERROR(exc.what());
	}

	player = game.world.create();
	vray::CompTransform playerTransform;
	playerTransform.setPosition({ 0.0f, 40.0f, 0.0f });

	auto& transform = game.world.emplace<vray::CompTransform>(player, playerTransform);

	camera = &game.world.emplace<vray::CompCamera>(
		player,
		engine.cameraSystem.createCamera(100.0f, 0.1f, 300.0f)
	);

	engine.cameraSystem.setActiveCamera(camera);

	playerController = new PlayerController(this, player);
	playerController->setEnabled(true);

	freeCamera.setGame(this);
	freeCamera.setLocked(true);
	freeCamera.setMoveSpeed(1.0f);

	spawner = new Spawner(game);

	spawner->spawnMap(maps.get("light"), { 0.0f, 1.0f, 1.0f });

	game.world.view<CompMapSpawn>().each([this] (entt::entity entity, CompMapSpawn& spawn) {
		spawns.push_back(entity);
	});

	respawn();
	//spawner->spawnPlatformGrid({ 0.0f, 0.0f, 0.0f }, { 2.0f, 0.3 }, 20);
	//spawner->spawnLight({ 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
}

SpatialTest::~SpatialTest() {
	delete spawner;
	delete playerController;
}

void SpatialTest::respawn() {
	if (spawns.empty()) return;
	entt::entity spawn = spawns[rand() % spawns.size()];
	auto& position = game.world.get<vray::CompTransform>(spawn).getPosition();
	playerController->moveTo(position + glm::vec3(0.0f, 0.15f, 0.0f));
}

void SpatialTest::update() {	
	playerController->update(deltaTime());

	if (!console->isOpened() && !playerController->isEnabled()) freeCamera.update(camera);
	else {
		const auto& position = game.world.get<vray::CompTransform>(player).getPosition();
		camera->setPosition(position + glm::vec3(0.0f, 1.8f, 0.0f));
	}
}

void SpatialTest::onEvent(vray::Event& evt) {
	if (!console->isOpened()) freeCamera.rotate(camera, evt);

	vray::EventDispatcher dispatcher(evt);
	dispatcher.fire<vray::KeyPressedEvent>([this](vray::KeyPressedEvent evt) {
		if (evt.getKeyCode() != VR_KEY_R) return false;
		respawn();
		return true;
	});

	dispatcher.fire<vray::KeyPressedEvent>([this](vray::KeyPressedEvent evt) {
		if (evt.getKeyCode() == VR_KEY_ESCAPE) {
			console->setOpened(!console->isOpened());
			return true;
		}
		else if (evt.getKeyCode() == VR_KEY_TAB) {
			vray::InputService::CursorMode cursorMode = engine.inputService.getCursorMode();
			if (cursorMode == vray::InputService::CursorMode::DISABLED)
				engine.inputService.setCursorMode(vray::InputService::CursorMode::NORMAL);
			else
				engine.inputService.setCursorMode(vray::InputService::CursorMode::DISABLED);
			return true;
		}
		return false;
	});

	static bool toggle = false;
	static glm::vec3 prev = glm::vec3(0.0f);

	dispatcher.fire<vray::KeyPressedEvent>([this](vray::KeyPressedEvent evt) {
		if (!toggle && evt.getKeyCode() == VR_KEY_W) {
			toggle = true;
			prev = game.world.get<vray::CompTransform>(player).getPosition();
			//VR_LOGIMPORTANT(glm::to_string(prev));
		}
		return toggle;
	});

	dispatcher.fire<vray::KeyReleasedEvent>([this](vray::KeyReleasedEvent evt) {
		if (evt.getKeyCode() == VR_KEY_W) {
			toggle = false;
			glm::vec3 curr = game.world.get<vray::CompTransform>(player).getPosition();
			//VR_LOGIMPORTANT(glm::to_string(prev) + " " + glm::to_string(curr) + " " + STR(glm::distance(prev, curr)));
		}
		return toggle;
	});
}

void SpatialTest::setupCommands() {
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
}

void SpatialTest::loadAssets() {
	game.meshes.load("models/cube.obj", "cube");
	game.textures.load("textures/KAMEN.JPG", "stone_bricks");
	game.textures.load("textures/default.png", "default");
	maps.load("maps/test.txt", "light");
	maps.load("maps/test.txt", "heavy");
}

VR_IMPLEMENT_GAME(SpatialTest);