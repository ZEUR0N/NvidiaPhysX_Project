#pragma once
#include "PlayState.h"
#include <conio.h>

class GameStateManager {
public:
	GameStateManager(PxPhysics* _gPhysics, PxScene* _gScene);
	~GameStateManager();

	void update(float t);
	void handleInput(unsigned char key);
private:
	std::vector<GameState*> gameStates;
	GameState* currentState;

	PlayState* playState;
};
