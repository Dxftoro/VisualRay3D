#include <visualray.h>
#include <visualray/layer_service/imgui_layer.h>

class DraftGame : public vray::Game {
private:
	entt::entity teapot, cube;
	glm::vec3 position;
	float amplitude, frequency, timeAccumulator;

public:
	DraftGame() {
		DraftGame::setFpsLimit(70);
		vray::Mesh* teapotMesh = meshes.load("models/teapot.obj", "teapot");
		vray::Mesh* cubeMesh = meshes.load("models/cube.obj", "cube");
		vray::Texture* stoneBricks = textures.load("models/sponza/KAMEN.JPG", "stone_bricks");

		teapot = world.create();
		cube = world.create();

		vray::CompRenderable teapotRenderable(teapotMesh, textures.get("stone_bricks"));
		
		vray::CompTransform cubeTransform, teapotTransform;

		cubeTransform.setScale({ 5.0f, 5.0f, 5.0f });
		cubeTransform.setPosition({ -50.0f, 0.0f, -1.0f });

		teapotTransform.setRotation({ glm::radians(-90.0f), 0.0f, 0.0f});

		world.emplace<vray::CompTransform>(teapot, teapotTransform);
		world.emplace<vray::CompTransform>(cube, cubeTransform);

		world.emplace<vray::CompRenderable>(teapot, teapotRenderable);
		world.emplace<vray::CompRenderable>(cube,
			vray::CompRenderable(cubeMesh, textures.get("stone_bricks")));

		amplitude = 2.0f;
		frequency = 0.5f;
		timeAccumulator = 0.0f;
	}
	~DraftGame() {}

	inline void update() override {
		vray::CompTransform& transform = world.get<vray::CompTransform>(teapot);

		timeAccumulator += deltaTime();
		transform.setPosition({0.0f, 0.0f, 
			amplitude * glm::sin(timeAccumulator * frequency)});
	}
};

VR_IMPLEMENT_GAME(DraftGame);