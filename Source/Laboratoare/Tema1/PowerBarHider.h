#pragma once

#include "RenderData.h"
#include "BaseObject.h"
#include "Colors.h"

#define POWER_BAR_HIDER_MESH_MAIN ("power_bar_hider_mesh_main")

#define POWER_BAR_WIDTH		(20.0f)
#define POWER_BAR_LENGTH	(100.0f)

#define POWER_BAR_HIDER_COLOR (COLOR_BLACK)

class PowerBarHider : public BaseObject {

public:
	PowerBarHider();
	~PowerBarHider();

	void RevealAt(glm::vec2 position, float percentage);

	static std::vector<RenderData> GetRenderData();
	std::vector<std::string> GetMeshNames() override;
	
};