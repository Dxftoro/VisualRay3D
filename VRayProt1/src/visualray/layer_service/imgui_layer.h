#pragma once
#include "vrpch.h"
#include "kernel.h"

#include "game.h"
#include "layer_stack.h"

namespace vray {

	class VRAYLIB ImGuiLayer : public Layer {
	private:
		Window* window;
		double time;
		bool open;
		int fps;
		std::thread* fpsCounter;

	public:
		ImGuiLayer(Window* window);
		~ImGuiLayer();

		void onPush() override;
		void onPop() override;
		void onUpdate() override;
		void onEvent(Event& evt) override;
	};

}