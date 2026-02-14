#include "vrpch.h"
#include "console.h"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "logservice.h"

namespace vray {

	Console::Console() : initialized(false) {
		inputBuffer.resize(VR_CONSOLE_INPUT_BUFFER_SIZE);
	}

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

		ImGui::Begin("Console");
		float footerHeight = ImGui::GetFrameHeightWithSpacing() * 2;
		bool reclaimFocus = false;

		ImGui::PushItemWidth(-1);

		if (ImGui::BeginChild("Scrolling", ImVec2(0, -footerHeight), false,
		  ImGuiWindowFlags_HorizontalScrollbar)) {

			for (std::string& message : messages) {
				ImGui::TextUnformatted(message.c_str());
			}
		}
		ImGui::EndChild();

		ImGui::Separator();

		if (ImGui::InputText("##commandInput",
			&inputBuffer[0],
			(VR_CONSOLE_INPUT_BUFFER_SIZE),
			ImGuiInputTextFlags_EnterReturnsTrue)) {

			if (inputBuffer[0]) {
				addMessage(inputBuffer);
				inputBuffer[0] = '\0';
			    reclaimFocus = true;
			}
		
		}

		ImGui::PopItemWidth();

		if (reclaimFocus) ImGui::SetKeyboardFocusHere();

		ImGui::End();
	}

}