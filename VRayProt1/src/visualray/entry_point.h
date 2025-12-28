#pragma once

//#ifndef _WINDOWS_
//#include <windows.h>
//#endif

#ifdef VR_PLATFORM_WIN

#include <crtdbg.h>

int AllocHook(int allocType, void* userData, size_t size,
    int blockType, long requestNumber,
    const unsigned char* filename, int lineNumber) {
    static FILE* logFile = nullptr;

    if (blockType == _CRT_BLOCK) return 1;

    if (!logFile) {
        fopen_s(&logFile, "allocations.log", "w");
    }

    switch (allocType) {
    case _HOOK_ALLOC:
        fprintf(logFile, "[ALLOC] #%ld, size: %zu, at %s:%d\n",
            requestNumber, size, filename, lineNumber);
        break;

    case _HOOK_FREE: {
        fprintf(logFile, "[FREE] %zu #%ld, ptr: %p\n", size, requestNumber, userData);

        //static std::unordered_set<void*> freedPointers;
        //if (freedPointers.contains(userData)) {
        //    printf(" DOUBLE FREE DETECTED! #%ld, ptr: %p\n",
        //        requestNumber, userData);
        //    __debugbreak();
        //}
        //freedPointers.insert(userData);
        break;
    }

    case _HOOK_REALLOC:
        fprintf(logFile, "[REALLOC] #%ld, old: %p, new size: %zu\n",
            requestNumber, userData, size);
        break;
    }

    fflush(logFile);

    return 1;
}

	extern vray::Game* vray::vrayMain(int argc, char* argv[]);

	int main(int argc, char* argv[]) {
        //_CrtSetAllocHook(AllocHook);
		vray::LogService::init();
		auto game = vray::vrayMain(argc, argv);
		game->run();
		delete game;
		vray::LogService::destroy();
		return 0;
	}
#endif