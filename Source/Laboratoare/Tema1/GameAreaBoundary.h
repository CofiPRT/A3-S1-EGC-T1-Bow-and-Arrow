#pragma once

#include "RenderData.h"
#include "BaseObject.h"
#include "Colors.h"
#include "Globals.h"

#define BOUNDARY_COLOR (COLOR_BLACK)

#define GAME_AREA_BOUNDARY_MESH_MAIN "game_area_boundary_mesh_main"

class GameAreaBoundary : public BaseObject {

public:
	GameAreaBoundary();
	~GameAreaBoundary();

	static std::vector<RenderData> GetRenderData();
	std::vector<std::string> GetMeshNames() override;

};

