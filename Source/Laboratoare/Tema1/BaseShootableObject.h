#pragma once

#include "Arrow.h"
#include "Transform2D.h"

#define DECAY_ROTATION_SPEED (4.0f)

class BaseShootableObject : public BaseObject {

protected:
	BaseShootableObject();
	~BaseShootableObject();

	float GetGravitationalAcceleration() override;

public:

	virtual bool TestCollision(Arrow* arrow) = 0;
	virtual int GetScore() = 0;

};