#pragma once
#include "kernel.h"

#include "thirdparty/EnTT/entt.hpp"
#include "world/components.h"
#include "render_service/renderer.h"
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

		bool onWindowClosing(WindowCloseEvent& evt);
		void renderSubmit();

	public:
		entt::registry world;
	private:
		using VisibleGroup = decltype(world.group<CompTransform>(entt::get<CompRenderable>));
		VisibleGroup visibleGroup;

	public:
		ResourceManager<Mesh> meshes;
		ResourceManager<Texture> textures;

		Game();
		virtual ~Game();

		inline void run();
		inline void stop();
		virtual void update() = 0;

		void onEvent(Event& evt);

		Window* getWindow() const { return window; }

		inline void pushLayer(Layer* layer) { layerStack.pushLayer(layer); }
		inline void pushOverlay(Layer* overlay) { layerStack.pushOverlay(overlay); }

		inline void popLayer(Layer* layer) { layerStack.popLayer(); }
		inline void popOverlay(Layer* overlay) { layerStack.popOverlay(); }

		static float deltaTime();
		static float getFpsLimit() { return fpsLimit; };
		static void	setFpsLimit(int _fpsLimit) { fpsLimit = _fpsLimit; };
	};

	Game* vrayMain();
}