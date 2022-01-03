#pragma once

#include "BaseShootableObject.h"
#include "Bow.h"
#include "Transform2D.h"

#define SHURIKEN_MESH_BLADES ("shuriken_mesh_blades")
#define SHURIKEN_MESH_CENTER ("shuriken_mesh_center")

#define SHURIKEN_BLADE_LENGTH	(50.0f)
#define SHURIKEN_CENTER_RADIUS	(5.0f)

#define SHURIKEN_BLADE_COLOR	(COLOR_DARK_GRAY)
#define SHURIKEN_CENTER_COLOR	(COLOR_GRAY)

#define SHURIKEN_CENTER_TRIANGLE_COUNT	(20.0f)

#define SHURIKEN_IMPACT_BOW_FACTOR		(0.5f)
#define SHURIKEN_IMPACT_ARROW_FACTOR	(0.5f)
#define SHURIKEN_ROTATION_SPEED			(10.0f)

#define SHURIKEN_SCORE	(50.0f)

#define SHURIKEN_DECAY_LENGTH (1.5f)

#define SHURIKEN_MIN_SPEED	(400.0f)
#define SHURIKEN_MAX_SPEED	(600.0f)

#define SHURIKEN_SPAWN_WEIGHT (10.0f)

class Shuriken : public BaseShootableObject {

protected:
	void UpdateAlive(double deltaTimeSeconds) override;

	float GetDecayLength() override;
	bool IsInBounds() override;

public:
	Shuriken(glm::vec2 position, glm::vec2 velocity);
	~Shuriken();
	
	int GetScore() override;
	bool TestCollision(Arrow* arrow) override;
	bool TestCollision(Bow* bow);

	static std::vector<RenderData> GetRenderData();
	std::vector<std::string> GetMeshNames() override;

};