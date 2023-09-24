#include "PlayState.h"

PlayState::PlayState(PxPhysics* _gPhysics, PxScene* _gScene): GameState(_gPhysics, _gScene)
{
	//PxMaterial*mat = _gPhysics->createMaterial(0.f, 0.f, 1.f);
	//PxShape* shape = CreateShape(PxBoxGeometry(50, 1, 100));
	//PxRigidStatic* ground = gPhysics->createRigidStatic({ 0,0,0 });
	////ground->attachShape(*shape);
	//ground->createShape(PxBoxGeometry(50, 1, 100), *mat);
	//gScene->addActor(*ground);
	//RenderItem* item = new RenderItem(shape, ground, { 0.6,0.2,1,1 });
	level = new LevelGenerator(_gPhysics, _gScene);
}

void PlayState::update(float t)
{
	if (player != nullptr) player->update(t);
	level->update(t);
}

void PlayState::handleInput(unsigned char key)
{
	if(player != nullptr)player->handleInput(key);
	level->handleInput(key);
	if (toupper(key) == 'G') {
		PxTransform tr = PxTransform(0, 8, 0);
		player = new PlayerController(gPhysics, gScene, tr, Vector4(0, 1, 1, 1));
	}
}
