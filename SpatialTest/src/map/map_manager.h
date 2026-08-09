#pragma once

#include "map.h"
#include <visualray/managers/resource_manager.h>

class MapManager : public vray::ResourceManager<Map> {
public:
	using ResourceManager<Map>::ResourceManager;
};