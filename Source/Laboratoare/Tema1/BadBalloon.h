#pragma once

#include "BaseBalloonObject.h"
#include "Colors.h"

#define BAD_BALLOON_MESH_BODY	("bad_balloon_mesh_body")
#define BAD_BALLOON_MESH_KNOT	("bad_balloon_mesh_knot")
#define BAD_BALLOON_MESH_STRING ("bad_balloon_mesh_string")
#define BAD_BALLOON_MESH_SHINE	("bad_balloon_mesh_shine")

#define BAD_BALLOON_COLOR_BODY		(COLOR_MUSTARD_YELLOW)
#define BAD_BALLOON_COLOR_KNOT		(COLOR_MUSTARD_YELLOW)
#define BAD_BALLOON_COLOR_STRING	(COLOR_BLACK)
#define BAD_BALLOON_COLOR_SHINE		(COLOR_LIGHT_GRAY)

#define BAD_BALLOON_SCORE (-10)

#define BAD_BALLOON_SPAWN_WEIGHT (20.0f)

class BadBalloon : public BaseBalloonObject {

public:
	BadBalloon(glm::vec2 position);
	~BadBalloon();

	int GetScore() override;

	static std::vector<RenderData> GetRenderData();
	std::vector<std::string> GetMeshNames() override;
};