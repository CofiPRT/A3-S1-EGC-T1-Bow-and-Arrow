#pragma once

#include "Arrow.h"
#include "PowerBar.h"

#define BOW_MESH_STRING ("bow_mesh_string")
#define BOW_MESH_ARC	("bow_mesh_arc")

#define BOW_STRING_COLOR	(COLOR_DARK_GRAY)
#define BOW_ARC_COLOR		(COLOR_DARK_BROWN)

#define BOW_ARC_TRIANGLE_COUNT (20)

#define BOW_ARC_HEIGHT	(200.0f)
#define BOW_ARC_WIDTH	(150.0f)

#define BOW_START_X (GAME_AREA_MIN_X + BOW_ARC_HEIGHT / 2.0)
#define BOW_START_Y ((GAME_AREA_MIN_Y + GAME_AREA_MAX_Y) / 2.0)

#define BOW_POWER			(2000.0f)
#define BOW_DRAW_SPEED		(1.0f)
#define BOW_MOVEMENT_SPEED	(500.0f)

#define BOW_POWER_BAR_OFFSET	(50.0f)

class Bow : public BaseObject {

private:
	enum class BowState { UNLOADED, LOADED, DRAWING};

	BowState currState = BowState::LOADED;

	Arrow* arrow = new Arrow();
	PowerBar* powerBar = new PowerBar();

	glm::vec2 target = glm::vec2(GAME_AREA_MAX_X, BOW_START_Y);
	float drawFactor = 0;

	void UpdatePowerBar();
	void UpdateDirection();
	void UpdateDraw(double deltaTimeSeconds);

	bool IsUnloaded();
	bool IsLoaded();
	bool IsDrawing();

protected:
	void UpdateAlive(double deltaTimeSeconds) override;

	bool IsInBounds() override;

public:
	Bow();
	~Bow();

	static std::vector<RenderData> GetRenderData();
	std::vector<std::string> GetMeshNames() override;
	std::vector<BaseObject*> GetObjectsToRender() override;

	void LoadArrow();
	void SetTarget(glm::vec2 target);
	void Draw();
	void Fire();
	void MoveUp(double deltaTime);
	void MoveDown(double deltaTime);

	Arrow* GetArrow();

};