#pragma once

#include "BaseObject.h"
#include "RenderData.h"
#include "RandomUtils.h"
#include "Transform2D.h"
#include "Colors.h"

#define HEART_MESH_BODY		("heart_mesh_body")
#define HEART_MESH_STROKE	("heart_mesh_stroke")

#define HEART_WIGGLE_LENGTH		(1.0f)
#define HEART_WIGGLE_FREQUENCY	(32.0f)
#define HEART_WIGGLE_MAGNITUDE	(5.0f)

#define HEART_JUMP_MIN_ANGLE	(-M_PI / 12)
#define HEART_JUMP_MAX_ANGLE	(M_PI / 12)
#define HEART_JUMP_VELOCITY		(100.0f)

#define HEART_WIDTH		(25.0f)
#define HEART_HEIGHT	(25.0f)

#define HEART_BODY_TRIANGLE_COUNT	(40)

#define HEART_COLOR_BODY	(COLOR_RED)
#define HEART_COLOR_STROKE	(COLOR_BLACK)

class Heart : public BaseObject {

private:
	bool wiggling = false;
	glm::vec2 initialPosition = glm::vec2(0);
	float wiggleTime = 0;

	std::uniform_real_distribution<float> decayJumpDist;

	void UpdateWiggle(double deltaTimeSeconds);

protected:
	void UpdateAlive(double deltaTimeSeconds) override;
	void UpdateDecay(double deltaTimeSeconds) override;

	float GetGravitationalAcceleration() override;
	bool IsInBounds();

public:
	Heart(glm::vec2 position);
	~Heart();

	void StartWiggle();
	void StartDecay();

	static std::vector<RenderData> GetRenderData();
	std::vector<std::string> GetMeshNames() override;

	bool IsWiggling();

};