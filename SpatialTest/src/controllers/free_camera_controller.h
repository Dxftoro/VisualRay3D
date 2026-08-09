#pragma once
#include <visualray/thirdparty/glm/glm.hpp>

namespace vray {
	class Game;
	class CompCamera;
	class Event;
}

class FreeCameraController {
private:
	vray::Game* ctx;
	float mouseSensivity;
	float moveSpeed;
	bool locked;

	void moveRotated(glm::vec3& position, const float angle, const float moveSpeed, const float deltaTime);

public:
	FreeCameraController() : ctx(nullptr), mouseSensivity(0.2f), moveSpeed(0.25f), locked(true) {}

	void setGame(vray::Game* ctx) { this->ctx = ctx; }
	void setMouseSensivity(float mouseSensivity) { this->mouseSensivity = mouseSensivity; }
	void setMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	void setLocked(bool locked);

	void update(vray::CompCamera* camera);
	void rotate(vray::CompCamera* camera, vray::Event& evt);

	bool isLocked() const { return locked; }
};