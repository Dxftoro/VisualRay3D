#pragma once
#include "kernel.h"
#include "vrpch.h"

#define VR_CONSOLE_INPUT_BUFFER_SIZE	512

namespace vray {

	class VRAYLIB Console {
	private:
		std::vector<std::string> messages;
		std::string inputBuffer;
		alignas(8) bool initialized;

	public:
		Console();

		inline void setup();
		void update();
		void addMessage(const std::string& message) { messages.push_back(message); }
	};

}