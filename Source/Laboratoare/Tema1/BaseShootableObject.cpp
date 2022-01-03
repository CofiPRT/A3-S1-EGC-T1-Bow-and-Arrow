#include "BaseShootableObject.h"

BaseShootableObject::BaseShootableObject() {}

BaseShootableObject::~BaseShootableObject() {}

float BaseShootableObject::GetGravitationalAcceleration() {
	return IsAlive() ? 0 : BaseObject::GetGravitationalAcceleration();
}
