#include "vrpch.h"
#include "fps_counter.h"
#include "game.h"

namespace vray {

	void FpsCounter::start(Window* window) {
		counterWorker = new std::thread([this, window] {
			while (!window->isClosed()) {
				fps = (int)(1.0f / Game::deltaTime());

				//std::ofstream fout("D:/fps_profiling.txt", std::ios::app);
				//fout << fps << std::endl;
				//fout.close();

				if (fps < minFps) minFps = fps;
				if (fps > maxFps) maxFps = fps;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

			VR_ENGINE_LOGINFO("Window caught closed!");
		});
	}

	void FpsCounter::stop() {
		counterWorker->join();
		delete counterWorker;
	}

}