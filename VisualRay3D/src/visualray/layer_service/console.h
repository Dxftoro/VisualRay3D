#pragma once
#include "kernel.h"
#include "vrpch.h"

#define VR_CONSOLE_INPUT_BUFFER_SIZE	512

namespace vray {

	struct VRAYLIB ConsoleCommand {
		using CommandCallback = std::function<void(const std::vector<std::string>&)>;
		CommandCallback callback;
		std::string description;
	};

	class VRAYLIB Console {
	private:
		std::unordered_map<std::string, ConsoleCommand> commands;
		std::vector<std::string> messages;
		std::string inputBuffer;
		bool initialized, opened;

	public:
		Console();

		void setup();
		void update();
		void write(const std::string& message) { messages.push_back("~ " + message); }
		void tokenize(const std::string& message, std::vector<std::string>& outArgs);
		void execute(const std::string& message);
		void open();
		void close();
	};

}