#include "BaseBalloonObject.h"

BaseBalloonObject::BaseBalloonObject() {

	deviationAngleDist = std::uniform_real_distribution<float>(
		BALLOON_MIN_DEVIATION_ANGLE,BALLOON_MAX_DEVIATION_ANGLE
	);

	deviationDelayDist = std::uniform_real_distribution<float>(
		BALLOON_MIN_DEVIATION_DELAY, BALLOON_MAX_DEVIATION_DELAY
	);

}

BaseBalloonObject::~BaseBalloonObject() {}

bool BaseBalloonObject::TestCollision(Arrow* arrow) {

	if (!IsAlive())
		return false;

	glm::vec2 direction = glm::normalize(position - arrow->GetPosition());
	glm::vec2 relativeDirection = glm::normalize(direction - rotation);

	float relativeAngle = atan2(relativeDirection.y, relativeDirection.x);

	float x = BALLOON_WIDTH / 2 * cos(relativeAngle);
	float y = BALLOON_HEIGHT / 2 * sin(relativeAngle);

	float balloonRadius = sqrt(x * x + y * y);

	bool result = glm::distance(position, arrow->GetPosition()) <= balloonRadius;

	if (result)
		currState = ObjectState::DECAYING;

	return result;

}

void BaseBalloonObject::UpdateAlive(double deltaTimeSeconds) {

	BaseObject::UpdateTrajectory(deltaTimeSeconds);

	glm::vec3 velocity3 = glm::vec3(0, BALLOON_ASCEND_SPEED, 0) * Transform2D::Rotate(currDeviation);
	velocity = glm::vec2(velocity3);

	// change towards target deviation
	currDeviation += (targetDeviation - currDeviation) * deltaTimeSeconds / BALLOON_DEVIATION_CHANGE_LENGTH;

	deviationDelay -= deltaTimeSeconds;

	// don't change deviation yet
	if (deviationDelay >= 0)
		return;

	targetDeviation = deviationAngleDist(RandomUtils::mt);
	deviationDelay = deviationDelayDist(RandomUtils::mt);

}

float BaseBalloonObject::GetDecayLength() {
	return BALLOON_DECAY_LENGTH;
}

bool BaseBalloonObject::IsInBounds() {

	float heightFactor = 1.0 / 2.0 + BALLOON_KNOT_HEIGHT_FACTOR + 2.0 * BALLOON_STRING_HEIGHT_FACTOR;
	return position.y <= GAME_AREA_MAX_Y + BALLOON_HEIGHT * heightFactor;

}