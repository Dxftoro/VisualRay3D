#pragma once
#include "kernel.h"

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

		Window* window;
		bool running;
		LayerStack layerStack;
		Renderer* renderer;

		bool onWindowClosing(WindowCloseEvent& evt);

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