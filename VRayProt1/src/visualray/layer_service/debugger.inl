#include "debugger.h"

namespace vray {

	template <typename T>
	T* Debugger::getVariable(const std::string& name) {
		auto it = varTable.find(name);
		if (it != varTable.end()) return (T*)varTable[name];
		else return nullptr;
	}

	template <typename T>
	void Debugger::addVariable(const std::string& name, T* value) {
		varTable[name] = (void*)value;
	}

}