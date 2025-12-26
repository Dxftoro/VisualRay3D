#pragma once

#ifdef VR_PLATFORM_WIN
	extern vray::Game* vray::vrayMain(int argc, char* argv[]);

	int main(int argc, char* argv[]) {
		vray::LogService::init();
		auto game = vray::vrayMain(argc, argv);
		game->run();
		delete game;
		vray::LogService::destroy();
		return 0;
	}
#endif