#include "free_camera_controller.h"
#include <visualray.h>

void FreeCameraController::moveRotated(
	glm::vec3& position,
	const float angle,
	const float moveSpeed,
	const float deltaTime) {

	position.x += cos(angle) * moveSpeed * deltaTime;
	position.z += sin(angle) * moveSpeed * deltaTime;
}

void FreeCameraController::update(vray::CompCamera* camera) {
	if (!locked) return;

	vray::EngineContext& engine = ctx->getEngineContext();
	vray::GameContext& game = ctx->getGameContext();
	vray::InputService& inputService = engine.inputService;

	glm::vec3 cameraPosition = camera->getPosition();
	float currentAngle = glm::radians(camera->getRotation().x);

	if (inputService.keyPressed(VR_KEY_W)) {
		moveRotated(cameraPosition, currentAngle, moveSpeed, ctx->deltaTime());
	}
	if (inputService.keyPressed(VR_KEY_S)) {
		moveRotated(cameraPosition, currentAngle + glm::pi<float>(), moveSpeed, ctx->deltaTime());
	}
	if (inputService.keyPressed(VR_KEY_A)) {
		moveRotated(cameraPosition, currentAngle - glm::pi<float>() / 2, moveSpeed, ctx->deltaTime());
	}
	if (inputService.keyPressed(VR_KEY_D)) {
		moveRotated(cameraPosition, currentAngle + glm::pi<float>() / 2, moveSpeed, ctx->deltaTime());
	}
	if (inputService.keyPressed(VR_KEY_Q)) {
		cameraPosition.y += moveSpeed * ctx->deltaTime();
	}
	if (inputService.keyPressed(VR_KEY_E)) {
		cameraPosition.y -= moveSpeed * ctx->deltaTime();
	}

	camera->setPosition(cameraPosition);
}

void FreeCameraController::rotate(vray::CompCamera* camera, vray::Event& evt) {
	if (!locked) return;

	vray::EngineContext& engine = ctx->getEngineContext();
	vray::GameContext& game = ctx->getGameContext();
	vray::InputService& inputService = engine.inputService;

	static glm::vec2 mouseBase(
		ctx->getWindow()->getWidth() * 0.5f,
		ctx->getWindow()->getHeight() * 0.5f
	);

	if (evt.getType() == vray::events::MOUSE_MOVED
		&& inputService.getCursorMode() == vray::InputService::CursorMode::DISABLED) {
		vray::MouseMovedEvent& moveEvt = dynamic_cast<vray::MouseMovedEvent&>(evt);
		double newX = moveEvt.getX();
		double newY = moveEvt.getY();

		glm::vec3 newRotation = camera->getRotation();
		newRotation.x += (newX - mouseBase.x) * mouseSensivity;
		newRotation.y += (mouseBase.y - newY) * mouseSensivity;

		mouseBase.x = newX;
		mouseBase.y = newY;

		if (newRotation.x < 0.0f) newRotation.x += 360.0f;
		if (newRotation.x > 360.0f) newRotation.x -= 360.0f;

		if (newRotation.y < -89.0f) newRotation.y = -89.0f;
		if (newRotation.y > 89.0f) newRotation.y = 89.0f;

		camera->setRotation(newRotation);
	}
}

void FreeCameraController::setLocked(bool locked) {
	vray::EngineContext& engine = ctx->getEngineContext();
	vray::InputService& inputService = engine.inputService;
	inputService.setCursorMode(
		locked ? 
			vray::InputService::CursorMode::DISABLED : 
			vray::InputService::CursorMode::NORMAL
	);
	inputService.setRawMouseInputEnabled(locked);
	this->locked = locked;
}