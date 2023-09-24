#pragma once
#include "../GameObjects/ParticleSystem.h"
#include "ForceField.h"

class FieldSystem
{
public:
	FieldSystem(std::vector<Particle*>* particles,BodySystem* bodySys, ParticleForceRegistry* fRegistry);
	~FieldSystem();

	void createSphereForceField(float const& radius, float const& distance, Field fieldType, Vector3 origin = Vector3(0));
	void createBoxForceField(Vector3 const& dimensions, float const& distance, Field fieldType, Vector3 origin = Vector3(0));

	void update(double t);
private:
	std::vector<Particle*>* particles_;

	BodySystem* bodySys_;

	std::vector<ForceField*> fields_;
	ParticleForceRegistry* fRegistry_;
};
