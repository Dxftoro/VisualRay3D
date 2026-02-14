#pragma once
#include "kernel.h"

#include "thirdparty/EnTT/entt.hpp"
#include "world/components.h"
#include "world/systems.h"
#include "render_service/renderer.h"
#include "physics_service/physics.h"
#include "window_service/window.h"
#include "input_service/input_service.h"

#include "event_service/event.h"
#include "event_service/game_events.h"
#include "layer_service/layer_stack.h"
#include "managers/resource_manager.h"

namespace vray {

	class Debugger;

	struct VRAYLIB EngineContext {
		std::unique_ptr<Window> window;
		//LayerStack layerStack;
		Debugger* debugger;
		Renderer* renderer;
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
		ResourceManager<Mesh> meshes;
		ResourceManager<Texture> textures;

		GameContext(const GameContext&) = delete;
		GameContext& operator=(const GameContext&) = delete;
		GameContext() = default;
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
		void renderSubmit();
		void onEventInternal(Event& evt);

		using VisibleGroup = decltype(gameContext.world.group<CompTransform>(entt::get<CompRenderable>));
		VisibleGroup visibleGroup;

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

		//inline void pushLayer(Layer* layer) { engineContext.layerStack.pushLayer(layer); }
		//inline void pushOverlay(Layer* overlay) { engineContext.layerStack.pushOverlay(overlay); }

		//inline void popLayer(Layer* layer) { engineContext.layerStack.popLayer(); }
		//inline void popOverlay(Layer* overlay) { engineContext.layerStack.popOverlay(); }

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