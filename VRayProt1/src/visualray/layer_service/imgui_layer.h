#pragma once
#include "vrpch.h"
#include "kernel.h"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "layer_stack.h"

namespace vray {

	class ImGuiLayer : public Layer {
	private:
		Window* window;
		double time;
		bool open;

	public:
		ImGuiLayer(Window* window);
		~ImGuiLayer();

		void onPush() override;
		void onPop() override;
		void onUpdate() override;
		void onEvent(Event& evt) override;
	};

}