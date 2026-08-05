#pragma once

/* ------- !!! For external including only !!! ------- */

#ifdef _WIN32
#define VR_PLATFORM_WIN
#endif

#define VR_VERSION	"v1.0.0-alpha"
#define VR_IMPLEMENT_GAME(GAME) vray::Game* vray::vrayMain(int argc, char* argv[]) { return vray::createGame<GAME>(argc, argv); }

/* ------- Engine files ------- */
#include "visualray/kernel.h"
#include "visualray/game.h"
#include "visualray/logger.h"
#include "visualray/logservice.h"
#include "visualray/input_codes.h"
#include "visualray/input_service/input_service.h"
#include "visualray/event_service/event.h"
#include "visualray/event_service/mouse_events.h"
#include "visualray/event_service/keyboard_events.h"
#include "visualray/event_service/physics_events.h"
#include "visualray/window_service/window.h"
#include "visualray/audio_service/audio.h"
#include "visualray/render_service/renderer.h"
#include "visualray/physics_service/physics.h"
#include "visualray/layer_service/debugger.h"
#include "visualray/layer_service/console.h"
#include "visualray/util/util.h"

/* ------- Third-party ------- */
#include "visualray/thirdparty/glm/glm.hpp"
#include "visualray/thirdparty/EnTT/entt.hpp"

#ifdef VR_ENTRY_POINT
#include "visualray/entry_point.h"
#endif