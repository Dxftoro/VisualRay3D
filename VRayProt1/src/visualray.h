#pragma once

/* ------- !!! For external including only !!! ------- */

#ifdef _WIN32
	#define VR_PLATFORM_WIN
#endif

#define VR_START	vray::Game*

/* ------- Engine files ------- */
#include "visualray/game.h"
#include "visualray/logger.h"
#include "visualray/logservice.h"
#include "visualray/input_codes.h"
#include "visualray/input_service/input_service.h"
#include "visualray/event_service/event.h"
#include "visualray/window_service/window.h"
#include "visualray/render_service/renderer.h"

#include "visualray/entry_point.h"