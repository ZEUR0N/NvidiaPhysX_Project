#include "BodyForces.h"

BodyWind::BodyWind(const Vector3 wind) : w (wind)
{}

void BodyWind::updateForce(SolidBody * body, float t)
{
	body->force += w;
}

//--------------------------------------------------

BodyTorque::BodyTorque(const Vector3 _torque) : torque(_torque)
{}

void BodyTorque::updateForce(SolidBody * body, float t)
{
	body->torque += torque;
}
