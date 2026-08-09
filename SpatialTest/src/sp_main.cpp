#define VR_ENTRY_POINT
#include <visualray.h>

#include "spatial_test.h"
#include "spawner.h"
#include "controllers/player_controller.h"

SpatialTest::SpatialTest()
:	Game(vray::WindowParams("Spatial test", 1290, 723)),
	engine(getEngineContext()),
	game(getGameContext()) {

	SpatialTest::setFpsLimit(70);
	console = engine.debugger->getConsole();

	try {
		loadAssets();
	}
	catch (std::exception& exc) {
		VR_ENGINE_LOGERROR(exc.what());
	}

	player = game.world.create();
	vray::CompTransform playerTransform;
	playerTransform.setPosition({ 0.0f, 40.0f, 0.0f });

	game.world.emplace<vray::CompTransform>(player, playerTransform);

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

	spawner->spawnMap(maps.get("test"), { 0.0f, 1.0f, 1.0f });

	//spawner->spawnPlatformGrid({ 0.0f, 0.0f, 0.0f }, { 2.0f, 0.3 }, 20);
	//spawner->spawnLight({ 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
}

SpatialTest::~SpatialTest() {
	delete spawner;
	delete playerController;
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
}

void SpatialTest::loadAssets() {
	game.meshes.load("models/cube.obj", "cube");
	game.textures.load("textures/KAMEN.JPG", "stone_bricks");
	game.textures.load("textures/default.png", "default");

	maps.load("maps/test.txt", "test");
}

VR_IMPLEMENT_GAME(SpatialTest);