#pragma once
#include "ParticleForceGenerator.h"
#include "BodyForceGenerator.h"

class ParticleForceRegistry
{
protected:
	// Storage for generator-particle entry
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* fg;
	}; 
	
	struct BodyForceRegistration
	{
		SolidBody* body;
		BodyForceGenerator* bfg;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;
	typedef std::vector<BodyForceRegistration> Registry_rigid;
	Registry_rigid registrations_rigid;
public:
	// Associate generator with a particle
	void add(Particle* particle, ParticleForceGenerator* fg);
	// Associate generator with a particle
	void addNews(BodySystem* bodySys, std::vector<BodyForceGenerator*> fg);
	// Remove association
	void remove(Particle* particle, ParticleForceGenerator* fg);
	// Remove body association
	void removeNews(SolidBody* body, BodyForceGenerator* bfg);
	//remove all associations of a given particle
	void clearParticle(Particle* particle);
	//remove all associations of a given body
	void clearBody(BodySystem* bodySys);
	//remove all associations of a given force
	void clearForce(ParticleForceGenerator* fg);
	//remove all associations of a given body force
	void clearBodyForce(BodyForceGenerator* bfg);
	// Removes all associations. Particle and Generators won't be deleted
	void clear();
	// Update all the generators in the registry
	void updateForces(float t);
};
