#pragma once

#include "BaseObject.h"
#include "RenderData.h"
#include "Globals.h"
#include "Colors.h"

#define ARROW_MESH_BODY			("arrow_mesh_body")
#define ARROW_MESH_TIP			("arrow_mesh_tip")
#define ARROW_MESH_TIP_SHADOW	("arrow_mesh_tip_shadow")
#define ARROW_MESH_TAIL			("arrow_mesh_tail")

#define ARROW_Y_MIN (GAME_AREA_MIN_Y)
#define ARROW_Y_MAX (GAME_AREA_MAX_Y)
#define ARROW_X_MAX (GAME_AREA_MAX_X)

#define ARROW_PENETRATION_LENGTH (10.0f)

#define ARROW_STUCK_ANIMATION_MAGNITUDE_FACTOR	(0.05f)
#define ARROW_STUCK_ANIMATION_FREQUENCY_FACTOR	(40.0f)
#define ARROW_STUCK_ANIMATION_LENGTH_FACTOR		(0.5f)

#define ARROW_BODY_LENGTH		(125.0f)
#define ARROW_BODY_WIDTH		(4.0f)

#define ARROW_TAIL_LENGTH		(20.0f)
#define ARROW_TAIL_START_WIDTH	(4.0f)
#define ARROW_TAIL_END_WIDTH	(20.0f)

#define ARROW_TIP_ARC_TRIANGLE_COUNT	(10)
#define ARROW_TIP_ARC_OFFSET_X			(40.0f)
#define ARROW_TIP_ARC_RADIUS_X			(10.0f)

#define ARROW_TIP_SHADOW_LENGTH			(10.0f)
#define ARROW_TIP_SHADOW_OUTER_WIDTH	(30.0f)
#define ARROW_TIP_SHADOW_INNER_WIDTH	(24.0f)

#define ARROW_COLOR_BODY		(COLOR_BROWN)
#define ARROW_COLOR_TAIL		(COLOR_LIGHT_GRAY)
#define ARROW_COLOR_TIP			(COLOR_LIGHT_GRAY)
#define ARROW_COLOR_TIP_SHADOW	(COLOR_GRAY)

class Bow; // forward declaration

class Arrow : public BaseObject {

private:
	enum class ArrowState { IDLE, LOADED, FLYING, STUCK };

	ArrowState currState = ArrowState::IDLE;

	Bow* bow = NULL;

	glm::vec2 initialRotation = glm::vec2(1, 0);
	float timeStuck = 0;
	float stuckAnimationMagnitude = 1.0;
	float stuckAnimationFrequency = 1.0;
	float stuckAnimationLength = 1.0;

	void UpdateLoadPosition();
	void UpdateFlight(double deltaTimeSeconds);
	void UpdateStuckAnimation(double deltaTimeSeconds);
	void StartStuckAnimation();

protected:
	void UpdateAlive(double deltaTimeSeconds) override;

	bool IsInBounds() override;

public:
	Arrow();
	~Arrow();

	static std::vector<RenderData> GetRenderData();
	std::vector<std::string> GetMeshNames() override;

	void Launch(glm::vec2 velocity);
	void LoadBy(Bow* bow);

	bool IsFlying();
	bool IsLoaded();
	bool IsStuck();
	bool IsIdle();

};