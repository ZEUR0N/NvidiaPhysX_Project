#pragma once
#include "../GameObjects/BodySystem.h"

class BodyForceGenerator
{
public:
	// Overload to provide functionality
	virtual void updateForce(SolidBody* body, float t) = 0;
};