#include "vrpch.h"
#include "console.h"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "logservice.h"

namespace vray {

	//Console::Console() : initialized(false) {
	//	// Â console.cpp (â DLL)
	//}

	void Console::setup() {
		ImVec2 windowSize = ImGui::GetMainViewport()->Size;
		ImGui::SetNextWindowPos({ 5, 5 }, ImGuiCond_Once);
		ImGui::SetNextWindowSize({ windowSize.x * 0.5f, windowSize.y * 0.95f }, ImGuiCond_Once);
	}

	void Console::update() {
		if (!initialized) {
			setup();
			initialized = true;
		}

		float footerHeight = ImGui::GetFrameHeightWithSpacing() * 2;
		bool reclaimFocus = false;
		
		//ImGui::PushItemWidth(-1);

		ImGui::Begin("Console");



		ImGui::End();

		//ImGui::PopItemWidth();
	}

}