#include "vrpch.h"
#include "kernel.h"

#include "fps_counter.h"
#include "console.h"

namespace vray {

	class Window;

	template <typename T>
	class VRAYLIB DebugDisplay {
	public:
		static void display(const std::string& name, const T* data);
	};

	class DebugTypeRegistry {
	private:
		template <typename T>
		struct TypeInfo {
			using DisplayFunc = void(*)(const std::string&, const T*);
			static constexpr DisplayFunc display = &DebugDisplay<T>::display;
		};

	public:
		template <typename T>
		static auto getDisplayFunc() { return TypeInfo<T>::display; }
	};

	struct DebugType {
		const void* data;
		void (*display)(const std::string&, const void*);

		DebugType() : data(nullptr), display(nullptr) {}

		template <typename T>
		DebugType(const T* _data) : data(_data) {
			display = [](const std::string& name, const void* ptr) {
				DebugDisplay<T>::display(name, (const T*)ptr);
			};
		}
	};

	class VRAYLIB Debugger {
	private:
		Window* window;
		FpsCounter fpsCounter;
		Console console;
		std::unordered_map<std::string, DebugType> varTable;
		float time;
		bool open;

	public:
		Debugger(Window* window);
		~Debugger();

		template <typename T>
		const T* getVariable(const std::string& name) const;

		template <typename T> 
		void addVariable(const std::string& name, const T* value);
		void removeVariable(const std::string& name);

		void update();
		void setOpen(bool open) { this->open = open; }

		bool isOpen() const { return open; }

		Console* getConsole() { return &console; }
	};

	template <typename T>
	const T* Debugger::getVariable(const std::string& name) const {
		auto it = varTable.find(name);
		if (it != varTable.end()) return (T*)varTable[name];
		else return nullptr;
	}

	template <typename T>
	void Debugger::addVariable(const std::string& name, const T* value) {
		varTable[name] = DebugType(value);
	}

}