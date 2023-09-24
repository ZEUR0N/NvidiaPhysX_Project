#pragma once
#include "ParticleForceGenerator.h"

class ParticleAnchoredSpring : public ParticleForceGenerator {
	//The other particle the spring is atached to
	Vector3 anchor;

	//Elasticity constant
	float& k;

	float restLength;

public:
	ParticleAnchoredSpring(Vector3 _anchor, float& _k, float _restLength);

	virtual void updateForce(Particle* particle, float t);
};
//-------------------------------------------------------------------------

class ParticleBungee : public ParticleForceGenerator {
	//The other particle the spring is atached to
	Particle* other;

	//Elasticity constant
	float k;

	float restLength;

public:
	ParticleBungee(Particle* _other, float _k, float _restLength);

	virtual void updateForce(Particle* particle, float t);
};
//-------------------------------------------------------------------------

class ParticleBuoyancy : public ParticleForceGenerator {
	float maxDepth;

	float volume;

	//Considering a water plane parallel to XZ
	float waterHeight;

	//Density of the liquid the object is submerged in
	float liquidDensity;

	//Posicion Y del liquido
	float posBuoyance;

public:
	ParticleBuoyancy(float _maxDepth, float _volume, float _waterHeight, float _liquidDensity, float yPos);

	virtual void updateForce(Particle* particle, float t);
};
//-------------------------------------------------------------------------

class ParticleDrag : public ParticleForceGenerator
{
	// Coefficient for velocity
	float k1;
	// Coefficient for squared velocity
	float k2;
public:
	ParticleDrag(float _k1, float _k2);
	virtual void updateForce(Particle* particle, float t);
};
//-------------------------------------------------------------------------

class ParticleGravity : public ParticleForceGenerator
{
	// Acceleration for gravity
	Vector3 g;
public:
	ParticleGravity(const Vector3& gravity);
	virtual void updateForce(Particle* particle, float t);
};
//-------------------------------------------------------------------------

class ParticleSpring : public ParticleForceGenerator {
	//The other particle the spring is atached to
	Particle* other;

	//Elasticity constant
	float& k;

	float restLength;

public:
	ParticleSpring(Particle* _other, float& _k, float _restLength);

	virtual void updateForce(Particle* particle, float t);
};
//-------------------------------------------------------------------------
