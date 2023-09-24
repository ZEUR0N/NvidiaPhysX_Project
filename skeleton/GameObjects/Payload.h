#pragma once
#include "ParticleSystem.h"

class Payload : ParticleSystem
{
public:
	Payload(physx::PxShape* sh, physx::PxTransform tr, Vector4 color = { 1,1,1,1 }, Vector3 vel = { 0,0,0 }, Vector3 acc = { 0,0,0 });
	virtual ~Payload() {};
	virtual void update(float t);
	virtual bool hasEnded() { return ParticleSystem::hasEnded(); };
	physx::PxTransform getTransform() const;
private:
	void randomValues();
};