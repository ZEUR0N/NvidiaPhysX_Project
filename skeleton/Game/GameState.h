#pragma once

#include "../utils/core.hpp"
#include "../utils/RenderUtils.hpp"
#include <vector>
#include "../utils/Color.h"
#include "../utils/Utils.h"

using namespace std;
using namespace physx;
using namespace Math;

class GameState {
public:
	GameState(PxPhysics* _gPhysics, PxScene* _gScene) : gPhysics(_gPhysics), gScene(_gScene) {};
	~GameState() {};
	virtual void update(float t) = 0;
	virtual void handleInput(unsigned char key) = 0;
protected:
	PxPhysics* gPhysics;
	PxScene* gScene;
};