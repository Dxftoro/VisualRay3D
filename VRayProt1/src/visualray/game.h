#pragma once
#include "kernel.h"

#include "thirdparty/EnTT/entt.hpp"
#include "world/components.h"
#include "world/systems.h"
#include "render_service/renderer.h"
#include "physics_service/physics.h"
#include "window_service/window.h"

#include "event_service/event.h"
#include "event_service/game_events.h"
#include "layer_service/layer_stack.h"
#include "managers/resource_manager.h"

namespace vray {
	class VRAYLIB Game {
	private:
		static float begTime;
		static float endTime;
		static float _deltaTime;
		static int fpsLimit;
		bool running;

		Window* window;
		LayerStack layerStack;
		Renderer* renderer;
		IPhysics* physics;
		IPhysicsDebugSystem* physicsDebugSystem;

		bool onWindowClosing(WindowCloseEvent& evt);
		void renderSubmit();
		void onEventInternal(Event& evt);

	public:
		entt::registry world;
	private:
		using VisibleGroup = decltype(world.group<CompTransform>(entt::get<CompRenderable>));
		VisibleGroup visibleGroup;

	public:
		CameraSystem cameraSystem;

		ResourceManager<Mesh> meshes;
		ResourceManager<Texture> textures;

		Game(const WindowParams& windowParams);
		~Game();

		inline void run();
		inline void stop();
		virtual void update() = 0;
		virtual void onEvent(Event& evt) = 0;

		Window* getWindow() const { return window; }

		inline void pushLayer(Layer* layer) { layerStack.pushLayer(layer); }
		inline void pushOverlay(Layer* overlay) { layerStack.pushOverlay(overlay); }

		inline void popLayer(Layer* layer) { layerStack.popLayer(); }
		inline void popOverlay(Layer* overlay) { layerStack.popOverlay(); }

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