#include "vrpch.h"
#include "kernel.h"

#include "fps_counter.h"

namespace vray {

	class Window;

	class VRAYLIB Debugger {
	private:
		Window* window;
		FpsCounter fpsCounter;
		std::unordered_map<std::string, void*> varTable;
		float time;
		bool open;

	public:
		Debugger(Window* window);
		~Debugger();

		template <typename T>
		T* getVariable(const std::string& name);

		template <typename T> 
		void addVariable(const std::string& name, T* value);
		void removeVariable(const std::string& name);

		void update();
		void setOpen(bool open) { this->open = open; }

		bool isOpen() const { return open; }
	};

}