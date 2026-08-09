#pragma once

#include <visualray.h>

#include "controllers/free_camera_controller.h"
#include "map/map_manager.h"

class Spawner;
class PlayerController;

class SpatialTest : public vray::Game {
private:
	vray::EngineContext& engine;
	vray::GameContext& game;
	vray::Console* console;
	vray::CompCamera* camera;
	entt::entity player;

	MapManager maps;
	Spawner* spawner;
	PlayerController* playerController;
	FreeCameraController freeCamera;
	std::vector<entt::entity> spawns;

	void loadAssets();
	void setupCommands();
	void respawn();

public:
	SpatialTest();
	~SpatialTest();

	void update() override;
	void onEvent(vray::Event& evt) override;

	entt::entity getPlayer() const { return player; }
};