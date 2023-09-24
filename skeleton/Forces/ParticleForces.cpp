#include "ParticleForces.h"

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3 _anchor, float& _k, float _restLength) : anchor(_anchor), k(_k), restLength(_restLength){}

void ParticleAnchoredSpring::updateForce(Particle* particle, float t)
{
	Vector3 f = particle->getPosition();
	f -= anchor;

	//Length
	float length = f.normalize();

	//Resulting force
	f *= -((length - restLength) * k);
	particle->addForce(f);
}
//----------------------------------------------------------------------------

ParticleBungee::ParticleBungee(Particle* _other, float _k, float _restLength) : other(_other), k(_k), restLength(_restLength){}

void ParticleBungee::updateForce(Particle* particle, float t)
{
	//Calculate distance vector
	Vector3 f = particle->getPosition();
	f -= other->getPosition();

	//Length
	float length = f.normalize();
	length = (length - restLength);
	if (length <= 0.0f)
		return;

	//Resulting dforce
	f *= -(length * k);
	particle->addForce(f);
}
//----------------------------------------------------------------------------

ParticleBuoyancy::ParticleBuoyancy(float _maxDepth, float _volume, float _waterHeight, float _liquidDensity, float yPos)
	: maxDepth(_maxDepth), volume(_volume), waterHeight(_waterHeight), liquidDensity(_liquidDensity), posBuoyance(yPos){}

void ParticleBuoyancy::updateForce(Particle* particle, float t)
{
	float depth;
	depth = particle->getPosition().y;
	Vector3 f(0.0f, 0.0f, 0.0f);
	float offsetY = -waterHeight / 2;
	if (depth > (maxDepth + posBuoyance + waterHeight - offsetY)) {
		particle->setColor({ 0.2, 1 , 0.2, 1 });
		return;
	}
	if (depth < (-waterHeight - maxDepth)) {
		particle->setColor({ 1, 0.5, 0, 1 });
		f.y = liquidDensity * volume;
	}
	else {
		float depthExt = -waterHeight + maxDepth;
		float volFactor = (depthExt - depth) / (2 * maxDepth);
		f.y = liquidDensity * volume * volFactor;
		particle->setColor({ 0.5,0,0.5,1 });
	}
	particle->addForce(f);

}

//----------------------------------------------------------------------------

ParticleDrag::ParticleDrag(float _k1, float _k2) : k1(_k1), k2(_k2) {}

void ParticleDrag::updateForce(Particle* particle, float t)
{
	Vector3 f = particle->getVelocity();

	//Drag coefficient
	float dragCoeff = f.normalize();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	//final force
	f *= -dragCoeff;
	particle->addForce(f);
}
//----------------------------------------------------------------------------

ParticleGravity::ParticleGravity(const Vector3& gravity) : g(gravity) {}

void ParticleGravity::updateForce(Particle* particle, float t)
{
	if (!particle->hasFiniteMass()) return;
	particle->addForce(g * particle->getMass());
}
//----------------------------------------------------------------------------

ParticleSpring::ParticleSpring(Particle* _other, float& _k, float _restLength) : other(_other), k(_k), restLength(_restLength) {}

void ParticleSpring::updateForce(Particle* particle, float t)
{
	//Calculate distance vector
	Vector3 f = particle->getPosition();
	f -= other->getPosition();

	//Length
	float length = f.normalize();

	//Resulting force
	float deltaL = (length - restLength);

	float forceMagnitude = -k * deltaL;
	f *= forceMagnitude;
	particle->addForce(f);
}
