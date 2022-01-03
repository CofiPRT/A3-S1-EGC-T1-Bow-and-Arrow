#pragma once

#include "BaseShootableObject.h"
#include "RandomUtils.h"

#define BALLOON_DECAY_LENGTH (2.0f)

#define BALLOON_HEIGHT				(100.0f)
#define BALLOON_WIDTH				(80.0f)

#define BALLOON_BODY_TRIANGLE_COUNT (40)

#define BALLOON_SHINE_START_ANGLE	(M_PI_2)
#define BALLOON_SHINE_END_ANGLE		(M_PI)
#define BALLOON_SHINE_START_PERCENT	(0.2f)
#define BALLOON_SHINE_END_PERCENT	(0.8f)

#define BALLOON_KNOT_WIDTH_FACTOR	(0.10f)
#define BALLOON_KNOT_HEIGHT_FACTOR	(0.05f)

#define BALLOON_STRING_WIDTH_FACTOR		(0.25f)
#define BALLOON_STRING_HEIGHT_FACTOR	(0.25f)
#define BALLOON_STRING_ROTATION			(M_PI / 12)

#define BALLOON_ASCEND_SPEED (200.0f)

#define BALLOON_MIN_DEVIATION_ANGLE (-M_PI_4)
#define BALLOON_MAX_DEVIATION_ANGLE (M_PI_4)

#define BALLOON_MIN_DEVIATION_DELAY (0.5f)
#define BALLOON_MAX_DEVIATION_DELAY (2.0f)

#define BALLOON_DEVIATION_CHANGE_LENGTH	(1.0f)

class BaseBalloonObject : public BaseShootableObject {

private:
	std::uniform_real_distribution<float> deviationAngleDist;
	std::uniform_real_distribution<float> deviationDelayDist;

	float currDeviation = 0;
	float deviationDelay = 0;
	float targetDeviation = 0;

protected:
	BaseBalloonObject();

	void UpdateAlive(double deltaTimeSeconds) override;

	float GetDecayLength() override;
	bool IsInBounds() override;

public:
	~BaseBalloonObject();

	bool TestCollision(Arrow* arrow) override;

};