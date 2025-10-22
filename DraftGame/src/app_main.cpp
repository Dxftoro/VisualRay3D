#include <visualray.h>

class DraftGame : public vray::Game {
private:
public:
	DraftGame() {
		
	}
	~DraftGame() {}

	inline void update() override {
		//VR_LOGINFO(std::to_string(Devshit::deltaTime()));
	}
};

vray::Game* vray::vrayMain() {
	return new DraftGame();
}