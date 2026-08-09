#pragma once
#include <visualray/thirdparty/glm/glm.hpp>
#include <visualray/thirdparty/EnTT/entt.hpp>

namespace vray {
	struct CompCamera;
	class Game;
}

class PlayerController {
private:
	vray::Game* ctx;
	entt::entity player;

	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 forward = glm::vec3(0.0f), right = glm::vec3(0.0f);
	float verticalVelocity = 0.0f;

	float playerSpeed = 0.0f;
	bool onGround = true;
	bool enabled = false;

	float maxSpeed = 8.13f;
	float acceleration = 10.0f;
	float airAcceleration = 0.7f;
	float friction = 4.0f;
	float stopSpeed = 2.54f;
	float jumpPower = 6.86f;
	float gravity = 20.32f;

	void handleKeys(float deltaTime);

public:
	PlayerController(vray::Game* ctx, entt::entity player);

	void calculateDirections(vray::CompCamera* camera);
	void accelerate(const glm::vec3& wishDir, float wishSpeed, float deltaTime);
	void airAccelerate(const glm::vec3& wishDir, float wishSpeed, float deltaTime);
	void frict(float deltaTime);
	void jump();
	void fall(float deltaTime);
	void update(float deltaTime);
	void moveTo(const glm::vec3& position);

	void setEnabled(bool enabled) { this->enabled = enabled; }
	bool isEnabled() const { return enabled; }
};