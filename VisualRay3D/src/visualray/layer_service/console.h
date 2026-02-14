#pragma once
#include "kernel.h"
#include "vrpch.h"

namespace vray {

	class VRAYLIB Console {
	private:
		std::vector<std::string> messages;
		std::string inputBuffer;
		alignas(8) bool initialized;

	public:
		Console() : initialized(false) {}

		inline void setup();
		void update();
	};

}