#include "player_controller.h"
#include <visualray.h>

PlayerController::PlayerController(vray::Game* _ctx, entt::entity _player)
	: ctx(_ctx), player(_player) {
}

void PlayerController::calculateDirections(vray::CompCamera* camera) {
	camera->calculateForward(forward);
	camera->calculateRight(right);
}

void PlayerController::accelerate(const glm::vec3& wishDir, float wishSpeed, float deltaTime) {
	float currentSpeed = glm::dot(velocity, wishDir);
	float addSpeed = wishSpeed - currentSpeed;

	if (addSpeed <= 0.0f) return;

	float accelSpeed = acceleration * wishSpeed * deltaTime;
	if (accelSpeed > addSpeed) accelSpeed = addSpeed;

	velocity += wishDir * accelSpeed;
}

void PlayerController::airAccelerate(const glm::vec3& wishDir, float wishSpeed, float deltaTime) {
	if (wishSpeed > 0.3f) wishSpeed = 0.3f;

	float currentSpeed = glm::dot(velocity, wishDir);
	float addSpeed = wishSpeed - currentSpeed;

	if (addSpeed <= 0.0f) return;

	float accelSpeed = airAcceleration * wishSpeed * deltaTime;
	if (accelSpeed > addSpeed) accelSpeed = addSpeed;

	velocity += wishDir * accelSpeed;
}

void PlayerController::frict(float deltaTime) {
	float speed = glm::length(glm::vec3(velocity.x, 0.0f, velocity.z));
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

void PlayerController::jump() {
	verticalVelocity = jumpPower;
	onGround = false;
}

void PlayerController::fall(float deltaTime) {
	verticalVelocity -= gravity * deltaTime;
}

void PlayerController::update(float deltaTime) {
	if (!enabled) return;

	vray::EngineContext& engine = ctx->getEngineContext();
	vray::GameContext& game = ctx->getGameContext();

	/* Detecting ground */
	const glm::vec3& position = game.world.get<vray::CompTransform>(player).getPosition();
	auto result = engine.physics->raycastFront(position + glm::vec3(0.0f, 0.5f, 0.0f), position);
	onGround = (result != std::nullopt);

	vray::CompTransform& transform = game.world.get<vray::CompTransform>(player);

	/* Handling keys */
	handleKeys(deltaTime);
	handleWalls(position);

	glm::vec3 newPosition = position + velocity * deltaTime;
	transform.setPosition(newPosition);
}

void PlayerController::handleKeys(float deltaTime) {
	vray::EngineContext& engine = ctx->getEngineContext();
	vray::GameContext& game = ctx->getGameContext();
	vray::InputService& inputService = engine.inputService;
	vray::CompCamera* camera = &game.world.get<vray::CompCamera>(player);

	float forwardMove = 0.0f, sideMove = 0.0f;
	if (inputService.keyPressed(VR_KEY_W)) forwardMove += 1.0f;
	if (inputService.keyPressed(VR_KEY_S)) forwardMove -= 1.0f;
	if (inputService.keyPressed(VR_KEY_A)) sideMove -= 1.0f;
	if (inputService.keyPressed(VR_KEY_D)) sideMove += 1.0f;

	calculateDirections(camera);
	
	glm::vec3 wishVel = (forward * forwardMove) + (right * sideMove);
	wishVel.y = 0.0f;
	
	glm::vec3 wishDir = glm::vec3(0.0f);

	float inputLen = glm::length(wishVel);
	float wishSpeed = 0.0f;

	if (inputLen > 0.0001f) {
		wishDir = wishVel / inputLen;
		wishSpeed = maxSpeed;
	}

	frict(deltaTime);
	if (onGround) {
		accelerate(wishDir, wishSpeed, deltaTime);
	}
	else {
		airAccelerate(wishDir, wishSpeed, deltaTime);
	}

	if (inputService.keyPressed(VR_KEY_SPACE) && onGround) {
		jump();
	}
	if (!onGround) {
		fall(deltaTime);
	}
	else verticalVelocity = 0.0f;

	playerSpeed = glm::length(velocity);
	velocity.y = verticalVelocity;
}

void PlayerController::handleWalls(const glm::vec3& position) {
	vray::EngineContext& engine = ctx->getEngineContext();

	glm::vec3 start = position + glm::vec3(0.0f, 0.5f, 0.0f);
	glm::vec3 end = start + glm::normalize(velocity) * 0.5f;

	auto result = engine.physics->raycastFront(
		start, end
	);

	if (result) {
		velocity -= result->hitNormal * glm::dot(velocity, result->hitNormal);
		//VR_LOGINFO("DASDASDASDASDASDASD");
		//engine.physicsDebugSystem->pushDebugLine(start, result->hitPoint);
	}
}

void PlayerController::moveTo(const glm::vec3& position) {
	vray::GameContext& game = ctx->getGameContext();

	velocity = glm::vec3(0.0f);
	verticalVelocity = 0.0f;

	auto& transform = game.world.get<vray::CompTransform>(player);
	transform.setPosition(position + glm::vec3(0.0f, 0.15f, 0.0f));
}