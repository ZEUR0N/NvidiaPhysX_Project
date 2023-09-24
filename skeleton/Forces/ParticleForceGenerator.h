#pragma once
#include "../GameObjects/Particle.h"
#include "../GameObjects/BodySystem.h"

class ParticleForceGenerator
{
public:
	// Overload to provide functionality
	virtual void updateForce(Particle* particle, float t) = 0;
};