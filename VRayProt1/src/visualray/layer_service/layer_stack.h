#pragma once
#pragma warning (disable: 4251)
#include "vrpch.h"
#include "kernel.h"
#include "../event_service/event.h"

namespace vray {

	class VRAYLIB Layer {
	protected:
	public:
		Layer() {}
		virtual ~Layer() {}

		virtual void onPush() {}
		virtual void onPop() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event& evt) {}
	};

	class LayerStack {
	private:
		std::vector<Layer*> layerContainer, overlayContainer;

	public:
		LayerStack() {}
		~LayerStack();

		VRAYLIB inline void pushLayer(Layer* layer);
		VRAYLIB inline void pushOverlay(Layer* layer);

		VRAYLIB inline void popLayer();
		VRAYLIB inline void popOverlay();

		VRAYLIB inline void update();
	};

}