#pragma once
#include "ParticleSystem.h"

class FireWorks : ParticleSystem
{
public:
	FireWorks(physx::PxShape* sh, physx::PxTransform tr, Vector3 vel = { 0,0,0 }, Vector3 acc = { 0,0,0 }, int fwtype = 0);
	virtual ~FireWorks() {};
	virtual void update(float t);
	virtual bool hasEnded() { return ParticleSystem::hasEnded(); };

private:
	float aux_dur;
	bool randomColors, updateColor;
	float ang;
	int fwType;

	void loadChar();
	void explosion();
	void charType1();
	void type1();
	void charType2();
	void type2();
	void charType3();
	void type3();
	void randomColor(bool x = true, bool y = true, bool z = true);
};