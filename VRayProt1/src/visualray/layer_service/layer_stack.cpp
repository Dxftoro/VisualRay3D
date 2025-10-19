#include "vrpch.h"
#include "layer_stack.h"

namespace vray {

	LayerStack::~LayerStack() {
		for (Layer* layer : layerContainer) delete layer;
		for (Layer* layer : layerContainer) delete layer;
	}

	inline void LayerStack::pushLayer(Layer* layer) { 
		layer->onPush();
		layerContainer.emplace_back(layer);
	}

	inline void LayerStack::pushOverlay(Layer* layer) {
		layer->onPush();
		overlayContainer.emplace_back(layer);
	}

	inline void LayerStack::popLayer() {
		layerContainer.back()->onPop();
		layerContainer.pop_back();
	}

	inline void LayerStack::popOverlay() {
		overlayContainer.back()->onPop();
		overlayContainer.pop_back();
	}

	inline void LayerStack::update() {
		for (Layer*& layer : layerContainer) layer->onUpdate();
		for (Layer*& overlay : overlayContainer) overlay->onUpdate();
	}

}