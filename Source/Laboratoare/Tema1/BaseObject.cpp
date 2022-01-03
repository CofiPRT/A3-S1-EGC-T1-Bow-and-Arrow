#include "BaseObject.h"

BaseObject::BaseObject() {}

BaseObject::~BaseObject() {}

float BaseObject::GetGravitationalAcceleration() {
	return GAME_DEFAULT_GRAVITATIONAL_ACC;
}

float BaseObject::GetDecayLength() {
	return GAME_DEFAULT_DECAY_LENGTH;
}

void BaseObject::UpdateTrajectory(double deltaTimeSeconds) {

	float g = GetGravitationalAcceleration();
	float t = deltaTimeSeconds;

	// x = x0 + V0x * t
	// y = y0 + V0y * t - (1/2)gt^2
	position += velocity * glm::vec2(t) - glm::vec2(0, (1 / 2) * g * t * t);

	// Vx = V0x
	// Vy = V0y - gt
	velocity -= glm::vec2(0, g * t);

}

void BaseObject::UpdateAlive(double deltaTimeSeconds) {}

void BaseObject::UpdateDecay(double deltaTimeSeconds) {

	decayTime += deltaTimeSeconds;

	if (decayTime >= GetDecayLength()) {
		currState = ObjectState::DEAD;
		return;
	}

	UpdateTrajectory(deltaTimeSeconds);
	
	// scale down
	scale = glm::vec2(1.0 - decayTime / GetDecayLength());

}

void BaseObject::Update(double deltaTimeSeconds) {

	if (!IsInBounds()) {
		currState = ObjectState::DEAD;
		return;
	}

	switch (currState) {
	case ObjectState::ALIVE:
		UpdateAlive(deltaTimeSeconds);
		break;
	case ObjectState::DECAYING:
		UpdateDecay(deltaTimeSeconds);
		break;
	default:
		break;
	}

}

std::vector<std::string> BaseObject::GetMeshNames() {
	return {};
}

std::vector<BaseObject*> BaseObject::GetObjectsToRender() {
	return {};
}

glm::vec2 BaseObject::GetPosition() {
	return position;
}

glm::vec2 BaseObject::GetRotation() {
	return rotation;
}

glm::vec2 BaseObject::GetScale() {
	return scale;
}

glm::vec2 BaseObject::GetVelocity() {
	return velocity;
}

void BaseObject::StartDecay() {
	if (!IsDecaying())
		currState = ObjectState::DECAYING;
}

bool BaseObject::IsInBounds() {
	return position.x >= GAME_AREA_MIN_X &&
		position.x <= GAME_AREA_MAX_X &&
		position.y >= GAME_AREA_MIN_Y &&
		position.y <= GAME_AREA_MAX_Y;
}

bool BaseObject::IsAlive() {
	return currState == ObjectState::ALIVE;
}

bool BaseObject::IsDecaying() {
	return currState == ObjectState::DECAYING;
}

bool BaseObject::IsDead() {
	return currState == ObjectState::DEAD;
}