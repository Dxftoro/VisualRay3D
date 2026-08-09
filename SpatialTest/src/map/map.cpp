#include "map.h"

BType resolveBType(char symbol) {
	switch (symbol) {
	case '*': return BType::HULL;
	case '#': return BType::BLOCK;
	case '.': return BType::VOID;
	case 'S': return BType::SPAWN;
	case 'L': return BType::LIGHT;
	default: return BType::HULL;
	}
}

Map::Map(const std::string& filename) : width(0), height(0) {
	std::ifstream fin(filename);
	if (!fin) {
		throw std::runtime_error("Cant't open file: " + filename);
	}

	std::string line;

	while (std::getline(fin, line)) {
		if (line.size() > width) width = line.size();
		for (char symbol : line) typeMap.push_back(resolveBType(symbol));
		height++;
	}

	fin.close();
}