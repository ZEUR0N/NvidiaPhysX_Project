#pragma once
#include "../Forces/FieldSystem.h"
#include "../Forces/BodyForces.h"

using namespace physx;

class Scene
{
public:
	Scene(PxPhysics* _gPhysics, PxScene* _gScene);
	~Scene();

	void update(const double& t);
	void keyPress(unsigned char key);

private:

	PxPhysics* gPhysics;
	PxScene* gScene;

	BodySystem* bodySys;
	ParticleSystem* particleSys;
	FieldSystem* fieldSys;

	std::vector<Particle*>* particles;

	ParticleForceRegistry* forceRegistry;

	float aux_lifetime = 0;
	RenderItem* anchor = nullptr;
	float k = 20.0;

	BodyWind* windForce = nullptr;
	BodyTorque* torqueGenerator = nullptr;
	bool activeForces = false;
};