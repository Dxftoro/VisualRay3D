#pragma once

#include "vrpch.h"

namespace vray::sleeping {

	void sleepUntil(std::chrono::steady_clock::time_point target);
	float ms(std::chrono::steady_clock::duration d);
	void beginTimerPrecision(uint32_t precision);
	void endTimerPrecision(uint32_t precision);

}