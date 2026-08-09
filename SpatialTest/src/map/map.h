#pragma once

#include <visualray/managers/resource.h>
#include "btype.h"

class Map : public vray::Resource {
private:
	std::vector<BType> typeMap;
	int width;
	int height;

public:
	Map(const std::string& filename);

	const std::vector<BType>& getTypeMap() const { return typeMap; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
};