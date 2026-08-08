#include "vrpch.h"
#include "sleeping.h"

#ifdef _WIN32
#include "windows.h"
#pragma comment(lib, "winmm.lib")
#endif

namespace vray::sleeping {
	
	void sleepUntil(std::chrono::steady_clock::time_point target) {
		while (true) {
			auto now = std::chrono::steady_clock::now();
			if (now >= target) return;

			auto remaining = target - now;
			if (remaining > std::chrono::milliseconds(2)) {
				std::this_thread::sleep_for(remaining - std::chrono::milliseconds(1));
			}
			else break;
		}

		while (std::chrono::steady_clock::now() < target) {
			std::this_thread::yield();
		}
	}

	float ms(std::chrono::steady_clock::duration d) {
		return std::chrono::duration<float, std::milli>(d).count();
	}

#ifdef _WIN32
	void beginTimerPrecision(uint32_t precision) {
		timeBeginPeriod(precision);
	}

	void endTimerPrecision(uint32_t precision) {
		timeEndPeriod(precision);
	}
#else
	void beginTimerPrecision(uint32_t precision) {}
	void endTimerPrecision(uint32_t precision) {}
#endif

}