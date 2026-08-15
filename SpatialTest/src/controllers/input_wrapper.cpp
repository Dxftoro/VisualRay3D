#include "input_wrapper.h"
#include <visualray.h>

InputWrapper::InputWrapper(vray::Game* _ctx)
:	ctx(_ctx) {
	console = ctx->getEngineContext().debugger->getConsole();
}

bool InputWrapper::keyPressed(int key) const {
	return !console->isOpened() && ctx->getEngineContext().inputService.keyPressed(key);
}