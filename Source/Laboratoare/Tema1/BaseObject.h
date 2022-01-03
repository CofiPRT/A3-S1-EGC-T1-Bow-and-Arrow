#pragma once

#include "Globals.h"

#include <Core/Engine.h>

class BaseObject {

protected:
	glm::vec2 position = glm::vec2(0);
	glm::vec2 rotation = glm::vec2(1, 0);
	glm::vec2 scale = glm::vec2(1);
	glm::vec2 velocity = glm::vec2(0);

	enum class ObjectState { ALIVE, DECAYING, DEAD };

	ObjectState currState = ObjectState::ALIVE;

	float decayTime = 0;

	BaseObject();
	~BaseObject();

	virtual void UpdateTrajectory(double deltaTimeSeconds);
	virtual void UpdateAlive(double deltaTimeSeconds);
	virtual void UpdateDecay(double deltaTimeSeconds);

	virtual float GetGravitationalAcceleration();
	virtual float GetDecayLength();
	virtual bool IsInBounds();

public:
	void Update(double deltaTimeSeconds);

	virtual std::vector<std::string> GetMeshNames();
	virtual std::vector<BaseObject*> GetObjectsToRender();

	glm::vec2 GetPosition();
	glm::vec2 GetRotation();
	glm::vec2 GetScale();
	glm::vec2 GetVelocity();

	virtual void StartDecay();

	bool IsAlive();
	bool IsDecaying();
	bool IsDead();

};