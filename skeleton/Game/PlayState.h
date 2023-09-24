#pragma once
#include "GameState.h"
#include "PlayerController.h"
#include "LevelGenerator.h"

class PlayState : public GameState {
public:
	PlayState(PxPhysics* _gPhysics, PxScene* _gScene);
	virtual ~PlayState() {};

	virtual void update(float t);
	virtual void handleInput(unsigned char key);

private:
	LevelGenerator* level;
	PlayerController* player = nullptr;
};