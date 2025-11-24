#include <visualray.h>
#include <visualray/layer_service/imgui_layer.h>

class DraftGame : public vray::Game {
private:
	entt::entity player, teapot, cube, plathform;
	vray::CompCamera* camera;

	float prevAngle, amplitude, frequency, timeAccumulator;
	
	inline void moveRotated(glm::vec3& cameraPosition, const float angle, const float moveSpeed) {
		cameraPosition.x += cos(angle) * moveSpeed * deltaTime();
		cameraPosition.z += sin(angle) * moveSpeed * deltaTime();
	}

	inline void handleKeys() {
		glm::vec3 cameraPosition = camera->getPosition();
		static float moveSpeed = 10.0f;
		float currentAngle = glm::radians(camera->getRotation().x);

		if (vray::InputService::keyPressed(VR_KEY_W)) {
			moveRotated(cameraPosition, currentAngle, moveSpeed);
		}
		if (vray::InputService::keyPressed(VR_KEY_S)) {
			moveRotated(cameraPosition, currentAngle + glm::pi<float>(), moveSpeed);
		}
		if (vray::InputService::keyPressed(VR_KEY_A)) {
			moveRotated(cameraPosition, currentAngle - glm::pi<float>() / 2, moveSpeed);
		}
		if (vray::InputService::keyPressed(VR_KEY_D)) {
			moveRotated(cameraPosition, currentAngle + glm::pi<float>() / 2, moveSpeed);
		}
		if (vray::InputService::keyPressed(VR_KEY_Q)) {
			cameraPosition.y += moveSpeed * deltaTime();
		}
		if (vray::InputService::keyPressed(VR_KEY_E)) {
			cameraPosition.y -= moveSpeed * deltaTime();
		}

		camera->setPosition(cameraPosition);
	}

	inline void handleRotation(vray::Event& evt) {
		static glm::vec2 mouseBase(getWindow()->getWidth() / 2, getWindow()->getHeight() / 2);

		if (evt.getType() == vray::EventType::MOUSE_MOVED) {
			//case EventType::MOUSE_CLICK: {
			//	//MouseClickEvent clickEvt = dynamic_cast<MouseClickEvent&>(evt);
			//	viewPicked = true;
			//	break;
			//}
			vray::MouseMovedEvent& moveEvt = dynamic_cast<vray::MouseMovedEvent&>(evt);

			glm::vec3 newRotation = camera->getRotation();
			newRotation.x += (moveEvt.getX() - mouseBase.x) / 5.0f;
			newRotation.y += (mouseBase.y - moveEvt.getY()) / 5.0f;

			if (newRotation.x < 0.0f) newRotation.x += 360.0f;
			if (newRotation.x > 360.0f) newRotation.x -= 360.0f;

			if (newRotation.y < -90.0f) newRotation.y = -90.0f;
			if (newRotation.y > 90.0f) newRotation.y = 90.0f;

			camera->setRotation(newRotation);
		}
	}

public:
	DraftGame() {
		DraftGame::setFpsLimit(70);
		vray::InputService::setMouseLocked(true);

		vray::Mesh* teapotMesh = meshes.load("models/teapot.obj", "teapot");
		vray::Mesh* cubeMesh = meshes.load("models/cube.obj", "cube");
		vray::Texture* stoneBricks = textures.load("textures/KAMEN.JPG", "stone_bricks");
		vray::Texture* defaultTexture = textures.load("textures/default.png", "default");

		player = world.create();
		teapot = world.create();
		cube = world.create();
		plathform = world.create();

		vray::CompRenderable teapotRenderable(teapotMesh, textures.get("stone_bricks"));
		vray::CompTransform cubeTransform, teapotTransform, plathformTransform;

		cubeTransform.setScale({ 5.0f, 5.0f, 5.0f });
		cubeTransform.setPosition({ -50.0f, 0.0f, -1.0f });

		teapotTransform.setRotation({ glm::radians(-90.0f), 0.0f, 0.0f});
		teapotTransform.setScale({ 0.25f, 0.25f, 0.25f });

		plathformTransform.setScale({ 20.0f, 0.25f, 20.f });

		camera = &world.emplace<vray::CompCamera>(player, vray::CompCamera(90.0f,
			getWindow()->getWidth(),
			getWindow()->getHeight(), 0.1f, 300.0f));
		cameraSystem.setActiveCamera(camera);

		world.emplace<vray::CompTransform>(teapot, teapotTransform);
		world.emplace<vray::CompTransform>(cube, cubeTransform);
		world.emplace<vray::CompTransform>(plathform, plathformTransform);

		world.emplace<vray::CompRenderable>(teapot, teapotRenderable);
		world.emplace<vray::CompRenderable>(cube,
			vray::CompRenderable(cubeMesh, textures.get("stone_bricks")));
		world.emplace<vray::CompRenderable>(plathform, vray::CompRenderable(cubeMesh, defaultTexture));

		prevAngle = 0.0f;
		amplitude = 2.0f;
		frequency = 0.5f;
		timeAccumulator = 0.0f;
	}
	~DraftGame() {}

	inline void update() override {
		handleKeys();

		vray::CompTransform& transform = world.get<vray::CompTransform>(teapot);

		timeAccumulator += deltaTime();
		transform.setPosition({0.0f, 0.0f, 
			amplitude * glm::sin(timeAccumulator * frequency)});
	}

	inline void onEvent(vray::Event& evt) { handleRotation(evt); }
};

VR_IMPLEMENT_GAME(DraftGame);

/*
¬ыбор решени€ - это действие над множеством альтернатив, 
в результате которого получаетс€ подмножество выбранных альтернатив

¬ыбор €вл. действием, предающим всей де€тельности целенаправленность.

’арактеристики выбора:
1.	ќценка альтернатив дл€ выбора может осуществл€тьс€ по одному или
	нескольким критери€м (имеющим количественный и/или качественный характер).
2.	¬ыбор может быть однократным или повтор€ющимс€.
3.	ѕоследстви€ выбора могут быть точно известны; иметь веро€тностный 
	характер (выбор в услови€х риска) или иметь неопр. исход (выбор в услови€х
	неопределЄнности).
4.	¬ыбор может быть односторонним или многосторонним.

”правление - функци€ организованных систем различной природы, обеспечивающа€
сохранение их определЄнной структуры, поддержание режима де€тельности,
реализацию программы, цели де€тельности.
*/