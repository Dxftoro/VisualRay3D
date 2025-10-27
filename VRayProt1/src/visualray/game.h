#pragma once
#include "kernel.h"

#include "thirdparty/EnTT/entt.hpp"
#include "world/components.h"
#include "render_service/renderer.h"
#include "window_service/window.h"

#include "event_service/event.h"
#include "event_service/game_events.h"
#include "layer_service/layer_stack.h"

namespace vray {
	class VRAYLIB Game {
	private:
		static float begTime;
		static float endTime;
		static float _deltaTime;
		bool running;

		Window* window;
		LayerStack layerStack;
		Renderer* renderer;

		entt::registry world;
		using VisibleGroup = decltype(world.group<TransformComponent>(entt::get<RenderableComponent>));
		VisibleGroup visibleGroup;

		bool onWindowClosing(WindowCloseEvent& evt);
		void renderSubmit();

	public:
		Game();
		virtual ~Game();

		inline void run();
		inline void stop();
		virtual void update() = 0;

		void onEvent(Event& evt);

		inline void pushLayer(Layer* layer) { layerStack.pushLayer(layer); }
		inline void pushOverlay(Layer* overlay) { layerStack.pushOverlay(overlay); }

		inline void popLayer(Layer* layer) { layerStack.popLayer(); }
		inline void popOverlay(Layer* overlay) { layerStack.popOverlay(); }

		static float deltaTime();
	};

	Game* vrayMain();
}