#pragma once
#include "../GameObjects/RigidBody.h"

class PlayerController {
public:
	PlayerController(PxPhysics* _gPhysics, PxScene* _gScene, PxTransform tr_, Vector4 color_);
	~PlayerController();

	void update(float t);
	void handleInput(char key);
private:
	PxPhysics* gPhysics;
	PxScene* gScene; 
	PxTransform tr;
	Vector4 color;
	RigidBody* player;

	bool pressed = false;

	float jumpForce = 500.0f, sideForce = 200.0f;
	const float yDeadPlayer = -8;
	float timer_ = 0, pressed_timer = 0.5f;
};

