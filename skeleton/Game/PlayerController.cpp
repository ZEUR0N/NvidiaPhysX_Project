#include "PlayerController.h"

PlayerController::PlayerController(PxPhysics* _gPhysics, PxScene* _gScene, PxTransform tr_, Vector4 color_) : gPhysics(_gPhysics), gScene(_gScene), tr(tr_), color(color_)
{
	player = new RigidBody(gPhysics,gScene, tr, color);
}

PlayerController::~PlayerController()
{
	delete player;
}

void PlayerController::update(float t)
{
	player->update(t);
	player->setColor((player->getPosition().y > 0) ? color : Vector4(0.8, 0.5, 0, 1.0));
	if (player->getPosition().y < yDeadPlayer || player->getPosition().y > 80) {
		delete player;
		player = new RigidBody(gPhysics, gScene, tr, color);
	}
	if (pressed && timer_ >= pressed_timer) {
		pressed = false;
		pressed_timer = timer_ + 0.5f;
	}
}

void PlayerController::handleInput(char key)
{
	switch (toupper(key))
	{
		case 'W':
		{
			if(player->getPosition().y < 10)
				player->addForce(Vector3(0, jumpForce, 0));
			break;
		}
		case 'A':
		{
			if (player->getPosition().x > -40)
				player->setPosition(Vector3(player->getPosition().x - 40, player->getPosition().y - 0.02, player->getPosition().z));
				//player->addForce(Vector3(-sideForce, 0, 0));
			break;
		}
		case 'D':
		{
			if (player->getPosition().x < 40)
				player->setPosition(Vector3(player->getPosition().x + 40, player->getPosition().y - 0.02, player->getPosition().z));
				//player->addForce(Vector3(sideForce, 0, 0));
			break;
		}
	}
}
