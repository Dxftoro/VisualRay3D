#include "vrpch.h"
#include "console.h"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "logservice.h"

namespace vray {

	Console::Console() : initialized(false), opened(false) {
		inputBuffer.resize(VR_CONSOLE_INPUT_BUFFER_SIZE);

		addCommand("help", [this](const std::vector<std::string>& args) {
			write("List of available commands:");
			for (auto& it : commands) {
				write(it.first + "\t- " + it.second.description);
			}
		},
		"This command. Prints descriptions of all other commands added.");

		addCommand("clear", [this](const std::vector<std::string>& args) {
			messages.clear();
		},
		"Cleans the console log.");
	}

	void Console::setup() {
		ImVec2 windowSize = ImGui::GetMainViewport()->Size;
		ImGui::SetNextWindowPos({ 5, 5 }, ImGuiCond_Once);
		ImGui::SetNextWindowSize({ windowSize.x * 0.5f, windowSize.y * 0.95f }, ImGuiCond_Once);
	}

	void Console::update() {
		if (!opened) return;
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
				execute(inputBuffer);
				inputBuffer[0] = '\0';
			    reclaimFocus = true;
			}
		
		}

		ImGui::PopItemWidth();

		if (reclaimFocus) ImGui::SetKeyboardFocusHere(-1);
		ImGui::End();
	}

	void Console::tokenize(const std::string& message, std::vector<std::string>& args) {
		std::string arg;
		for (char c : message) {
			if (c == ' ' || c == '\t') {
				args.push_back(arg);
				arg.clear();
				continue;
			}
			else if (c == '\0') break;

			arg += c;
		}

		if (!arg.empty()) args.push_back(arg);
	}

	void Console::execute(const std::string& message) {
		write(message);

		std::vector<std::string> args;
		tokenize(message, args);

		if (args.empty()) return;

		auto it = commands.find(args[0]);
		if (it != commands.end()) {
			it->second.callback(args);
		}
		else {
			write("Unknown command: " + args[0]);
		}
	}

	void Console::addCommand(
		const const std::string& name, 
		const CommandCallback& callback, 
		const std::string& description) {

		commands[name] = { callback, description };
	}

}