#pragma once
#include "kernel.h"

#include "render_service/camera_system.h"
#include "window_service/window.h"
#include "input_service/input_service.h"
#include "spatial_service/spatial_system.h"

#include "event_service/game_events.h"
#include "managers/resource_manager.h"

namespace vray {

	/* Forward declarations */
	class Debugger;
	class Renderer;
	class Audio;
	class IPhysics;
	class IPhysicsDebugSystem;

	struct VRAYLIB EngineContext {
		std::unique_ptr<Window> window;
		Debugger* debugger;
		Renderer* renderer;
		Audio* audio;
		IPhysics* physics;
		IPhysicsDebugSystem* physicsDebugSystem;
		CameraSystem cameraSystem;
		InputService inputService;

		EngineContext(const EngineContext&) = delete;
		EngineContext& operator=(const EngineContext&) = delete;
		EngineContext() : cameraSystem(nullptr) {}
	};

	struct VRAYLIB GameContext {
		entt::registry world;
		SpatialSystem space;
		ResourceManager<Mesh> meshes;
		ResourceManager<Texture> textures;
		ResourceManager<Sound> sounds;

		GameContext(const GameContext&) = delete;
		GameContext& operator=(const GameContext&) = delete;
		GameContext() : space(world) {}
	};

	class VRAYLIB Game {
	private:
		static float begTime;
		static float endTime;
		static float _deltaTime;
		static int fpsLimit;
		bool running;

		EngineContext engineContext;
		GameContext gameContext;

		bool onWindowClosing(WindowCloseEvent& evt);
		void onEventInternal(Event& evt);

	public:
		Game(const WindowParams& windowParams);
		~Game();

		inline void run();
		inline void stop();
		virtual void update() = 0;
		virtual void onEvent(Event& evt) = 0;

		Window* getWindow() const { return engineContext.window.get(); }
		EngineContext& getEngineContext() { return engineContext; }
		GameContext& getGameContext() { return gameContext; }

		void setClearColor(const glm::vec4& color);

		static float deltaTime();
		static float getFpsLimit() { return fpsLimit; };
		static void	setFpsLimit(int _fpsLimit) { fpsLimit = _fpsLimit; };
	};

	template<typename GameT>
	Game* createGame(int argc, char* argv[]) {
		if constexpr (requires { GameT(argc, argv); }) {
			return new GameT(argc, argv);
		}
		else {
			return new GameT();
		}
	}

	Game* vrayMain(int argc, char* argv[]);

}