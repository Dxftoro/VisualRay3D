#pragma once

#include <visualray.h>
#include "map/map.h"

class Spawner {
private:
	vray::GameContext& game;

	entt::entity spawnElement(const glm::vec3& position, const glm::vec3& size, const std::string& filename);

public:
	Spawner(vray::GameContext& _game) : game(_game) {}

	entt::entity spawnModel(const glm::vec3& pos, const std::string& resourceName);
	entt::entity spawnLight(const glm::vec3& pos, const glm::vec3& color);
	entt::entity spawnPlatform(const glm::vec3& position, const glm::vec2& size);
	
	void spawnPlatformGrid(const glm::vec3& center, const glm::vec2& cellSize, int size);
	void spawnMapBlock(BType btype, const glm::vec3& position, const glm::vec3& lightColor);
	void spawnMap(const Map* map, const glm::vec3& lightColor);
};