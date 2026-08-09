#pragma once

#include <visualray.h>

class Spawner {
private:
	vray::GameContext& game;

public:
	Spawner(vray::GameContext& _game) : game(_game) {}

	entt::entity spawnModel(const glm::vec3& pos, const std::string& resourceName);
	entt::entity spawnLight(const glm::vec3& pos, const glm::vec3& color);
	entt::entity spawnPlatform(const glm::vec3& position, const glm::vec2& size);
	void spawnPlatformGrid(const glm::vec3& center, const glm::vec2& cellSize, int size);
};