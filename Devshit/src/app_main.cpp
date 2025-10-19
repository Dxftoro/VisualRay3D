#include <visualray.h>

class Devshit : public vray::Game {
private:
public:
	Devshit() {
		
	}
	~Devshit() {}

	inline void update() override {
		//VR_LOGINFO(std::to_string(Devshit::deltaTime()));
	}
};

vray::Game* vray::vrayMain() {
	return new Devshit();
}