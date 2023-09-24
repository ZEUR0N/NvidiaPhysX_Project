#pragma once
#include "BodyForceGenerator.h"

class BodyWind : public BodyForceGenerator {
private:
	Vector3 w;
public:
	BodyWind(const Vector3 wind);
	virtual void updateForce(SolidBody* body, float t);
};
//--------------------------------------------------

class BodyTorque : public BodyForceGenerator {
private:
	Vector3 torque;
public:
	BodyTorque(const Vector3 _torque);
	virtual void updateForce(SolidBody* body, float t);
};
//--------------------------------------------------

