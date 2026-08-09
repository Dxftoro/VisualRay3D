#include "spawner.h"

entt::entity Spawner::spawnModel(const glm::vec3& pos, const std::string& resourceName) {
	entt::entity ent = game.world.create();

	vray::CompRenderable renderable(
		game.meshes.get(resourceName),
		game.textures.get("generic")
	);

	vray::CompTransform transform;
	transform.setPosition(pos);
	transform.setSize(renderable.mesh->getBaseSize());

	game.world.emplace<vray::CompTransform>(ent, transform);
	game.world.emplace<vray::CompRenderable>(ent, renderable);

	return ent;
}

entt::entity Spawner::spawnLight(const glm::vec3& pos, const glm::vec3& color) {
	entt::entity lightEntity = game.world.create();

	vray::CompTransform transform;
	transform.setPosition(pos);
	transform.setSize({ 0.5f, 0.5f, 0.5f });

	game.world.emplace<vray::CompPointLight>(lightEntity);

	vray::CompPointLightData& lightData = game.world.get<vray::CompPointLightData>(lightEntity);
	lightData.position = glm::vec4(pos, 1.0f);
	lightData.mergeColor(color);

	game.world.emplace<vray::CompTransform>(lightEntity, transform);

	return lightEntity;
}

entt::entity Spawner::spawnPlatform(const glm::vec3& position, const glm::vec2& size) {
	entt::entity platform = game.world.create();

	vray::CompTransform transform;
	//transform.setPosition({ 0.0f, 5.0f, 0.0f });
	transform.setPosition(position);
	transform.setSize({ size.x, size.y, size.x });

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

	return platform;
}

void Spawner::spawnPlatformGrid(const glm::vec3& center, const glm::vec2& _cellSize, int size) {
	int cellCount = size * size;
	float cellSize = _cellSize.x;

	const glm::vec3 cellStartPosition = {
		center.x - (cellSize * (size / 2)) + cellSize * 0.5,
		center.y,
		center.z - (cellSize * (size / 2)) + cellSize * 0.5
	};

	//VR_LOGINFO(std::to_string(cellStartPosition.x) + ", " + std::to_string(cellStartPosition.z));

	glm::vec3 cellPosition = cellStartPosition;
	spawnPlatform(cellPosition, _cellSize);

	for (int i = 0; i < cellCount - 1; i++) {
		if (i > 0 && (i + 1) % size == 0) {
			cellPosition.x = cellStartPosition.x;
			cellPosition.z += cellSize;
		}
		else cellPosition.x += cellSize;

		spawnPlatform(cellPosition, _cellSize);
	}
}
