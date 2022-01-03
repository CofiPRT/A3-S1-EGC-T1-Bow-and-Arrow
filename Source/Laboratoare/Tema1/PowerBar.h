#pragma once

#include "PowerBarHider.h"
#include "BaseObject.h"
#include "RenderData.h"
#include "Colors.h"

#define POWER_BAR_MESH_MAIN ("power_bar_mesh_main")

#define POWER_BAR_MIN_COLOR		(COLOR_GREEN)
#define POWER_BAR_MED_COLOR		(COLOR_YELLOW)
#define POWER_BAR_MAX_COLOR		(COLOR_RED)

class PowerBar : public BaseObject {

private:
	PowerBarHider* hider = new PowerBarHider();

public:
	PowerBar();
	~PowerBar();

	void RevealAt(glm::vec2 position, float percentage);

	static std::vector<RenderData> GetRenderData();
	std::vector<std::string> GetMeshNames() override;
	std::vector<BaseObject*> GetObjectsToRender() override;
	
};