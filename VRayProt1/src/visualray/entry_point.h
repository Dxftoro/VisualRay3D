#pragma once

#ifdef VR_PLATFORM_WIN
	extern vray::Game* vray::vrayMain();

	int main() {
		vray::LogService::init();
		auto game = vray::vrayMain();
		game->run();
		delete game;
		vray::LogService::destroy();
		return 0;
	}
#endif