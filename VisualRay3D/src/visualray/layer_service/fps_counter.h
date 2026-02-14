#pragma once
#include "vrpch.h"
#include "kernel.h"

namespace vray {

	class Window;

	class FpsCounter {
	private:
		int fps, minFps, maxFps;
		std::thread* counterWorker;

	public:
		FpsCounter() : fps(0.0f), minFps(9999999.0f), maxFps(0.0f), counterWorker(nullptr) {}

		int getFps() const { return fps; }
		int getMinFps() const { return minFps; }
		int getMaxFps() const { return maxFps; }

		void start(Window* window);
		void stop();
	};

}