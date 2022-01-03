#pragma once

#include "BaseBalloonObject.h"
#include "Colors.h"

#define GOOD_BALLOON_MESH_BODY	("good_balloon_mesh_body")
#define GOOD_BALLOON_MESH_KNOT	("good_balloon_mesh_knot")
#define GOOD_BALLOON_MESH_STRING ("good_balloon_mesh_string")
#define GOOD_BALLOON_MESH_SHINE	("good_balloon_mesh_shine")

#define GOOD_BALLOON_COLOR_BODY		(COLOR_DARK_RED)
#define GOOD_BALLOON_COLOR_KNOT		(COLOR_DARK_RED)
#define GOOD_BALLOON_COLOR_STRING	(COLOR_BLACK)
#define GOOD_BALLOON_COLOR_SHINE	(COLOR_LIGHT_GRAY)

#define GOOD_BALLOON_SCORE (20)

#define GOOD_BALLOON_SPAWN_WEIGHT (40.0f)

class GoodBalloon : public BaseBalloonObject {

public:
	GoodBalloon(glm::vec2 position);
	~GoodBalloon();

	int GetScore() override;

	static std::vector<RenderData> GetRenderData();
	std::vector<std::string> GetMeshNames() override;
};