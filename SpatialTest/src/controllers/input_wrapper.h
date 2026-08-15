#pragma once

namespace vray {
	class Game;
	class Console;
}

class InputWrapper {
private:
	vray::Game* ctx;
	vray::Console* console;

public:
	InputWrapper(vray::Game* ctx);

	bool keyPressed(int key) const;
};