#pragma once

#include <visualray.h>

#include "controllers/free_camera_controller.h"

class Spawner;
class PlayerController;

class SpatialTest : public vray::Game {
private:
	vray::EngineContext& engine;
	vray::GameContext& game;
	vray::Console* console;
	vray::CompCamera* camera;
	entt::entity player;

	Spawner* spawner;
	PlayerController* playerController;
	FreeCameraController freeCamera;

	void loadAssets();

public:
	SpatialTest();
	~SpatialTest();

	void update() override;
	void onEvent(vray::Event& evt) override;

	entt::entity getPlayer() const { return player; }
};