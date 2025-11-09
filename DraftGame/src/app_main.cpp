#include <visualray.h>
#include <visualray/layer_service/imgui_layer.h>

class DraftGame : public vray::Game {
private:
public:
	DraftGame() {
		DraftGame::setFpsLimit(70);
		vray::Mesh* teapotMesh = meshes.load("models/teapot.obj", "teapot");

		entt::entity teapot = world.create();
		vray::CompRenderable renderable(teapotMesh);

		world.emplace<vray::CompTransform>(teapot);
		world.emplace<vray::CompRenderable>(teapot, renderable);
	}
	~DraftGame() {}

	inline void update() override {
		
	}
};

VR_IMPLEMENT_GAME(DraftGame);