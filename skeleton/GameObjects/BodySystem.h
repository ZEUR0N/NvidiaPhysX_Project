#pragma once
#include <vector>

#include "../utils/core.hpp"
#include "../utils/RenderUtils.hpp"
#include "../utils/Utils.h"

using namespace physx;
using namespace std;
using namespace Math;

enum BodyType { 
	PLAYERCONTROLLER, BULLET, FALL
};

enum FallGenerator { 
	CERO, FREE, RAND 
};


struct SolidBody{
	PxRigidDynamic* rigid = nullptr;
	bool isNew;
	float life;
	RenderItem* item = nullptr;
	PxVec3 force;
	PxVec3 torque;
};

class BodySystem {
public:
	BodySystem(PxPhysics* _gPhysics, PxScene* _gScene, PxTransform pSet, 
		float stepSet = 1.5f, bool _colorR = true, float lifeSet = 8, float sizeSet = 1,
		int max = 100, Vector4 colorSet = { 1, 0, 0, 1.0 }, float damp = 0.9);
	~BodySystem();
	void addBody(BodyType bType, Vector3 vel = Vector3(0));
	void integrate(float t);
	void setFallGenerator(int type = -1);
	void setCameraPlayer();
	void handleInput(unsigned char key);

	vector<SolidBody*> bodies;
private:

	PxPhysics* gPhysics;
	PxScene* gScene;
	PxTransform p;
	float step, life, size, timeSinceAdding = 0;
	bool colorR;
	int maxParticles, numParticles = 0;
	Vector4 color;
	float damping;

	FallGenerator fall;
	float distanceOffset = 0.f;
	bool activeBodyPlayer = false;
	SolidBody* bodyPlayer = nullptr;
};
