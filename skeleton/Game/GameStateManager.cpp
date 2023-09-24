#include "GameStateManager.h"

GameStateManager::GameStateManager(PxPhysics* _gPhysics, PxScene* _gScene)
{
	playState = new PlayState(_gPhysics, _gScene);
	gameStates.push_back(playState);
	currentState = playState;
}

GameStateManager::~GameStateManager()
{
	for (GameState* gs : gameStates)
		delete gs;
	gameStates.clear();
}

void GameStateManager::update(float t)
{
	currentState->update(t);
}

void GameStateManager::handleInput(unsigned char key)
{
	currentState->handleInput(key);
}
