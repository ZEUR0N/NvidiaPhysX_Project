#include "BodySystem.h"

BodySystem::BodySystem(PxPhysics* _gPhysics, PxScene* _gScene, PxTransform pSet, float stepSet, bool _colorR, float lifeSet, float sizeSet, int max, Vector4 colorSet, float damp)
	: gPhysics(_gPhysics), gScene(_gScene), p(pSet), step(stepSet), colorR(_colorR), life(lifeSet), size(sizeSet), maxParticles(max), color(colorSet), damping(damp)
{
	fall = CERO;
}

BodySystem::~BodySystem()
{
	for (SolidBody* s : bodies)
		delete s;
	bodies.clear();
}

void BodySystem::addBody(BodyType bType, Vector3 vel)
{
	if (colorR) {
		color = Vector4(random(Vector3(0, 0, 0), Vector3(10, 10, 10)) / 10.0, 1.0);
	}
	float s = size, lifetime = life;

	SolidBody* body = new SolidBody;

	//rigid
	PxRigidDynamic* rigid = gPhysics->createRigidDynamic(p);
	

	switch (bType)
	{
	case PLAYERCONTROLLER:
		{
			PxRigidBodyExt::updateMassAndInertia(*rigid, 1);
			lifetime = 10000;
			bodyPlayer = body;
			break;
		}
	case BULLET:
		{
			distanceOffset = 5.0f;
			rigid->setGlobalPose(PxTransform(Vector3(GetCamera()->getTransform().p + GetCamera()->getDir() * distanceOffset),p.q));
			//Cinetica
			rigid->setLinearVelocity(vel);			rigid->setLinearDamping(0.0);
			rigid->setAngularVelocity({ 0,2,0 });	rigid->setAngularDamping(0.05);
			////Dinamica
			rigid->setMass(0.1);
			rigid->setMassSpaceInertiaTensor(PxVec3(0.f, 0.f, 1.f));
			//Tamaño
			s *= 0.3;
			break;
		}
	case FALL:
		{
			if(fall == FREE){
				PxRigidBodyExt::updateMassAndInertia(*rigid, 1);
			}
			else if (fall == RAND)
			{
				//Cinetica
				vel = Vector3(-5.0f + rand() / (RAND_MAX / (10.0f)), -5.0f + rand() / (RAND_MAX / (10.0f)), -5.0f + rand() / (RAND_MAX / (10.0f)));
				rigid->setLinearVelocity(vel);			rigid->setLinearDamping(0.0);
				rigid->setAngularVelocity({ 0,2,0 });	rigid->setAngularDamping(0.05);
				////Dinamica
				float randMass = random(3, 7) * 5;
				rigid->setMass(randMass);
				PxVec3 randInertia = random(Vector3(0), Vector3(10, 10, 10));
				rigid->setMassSpaceInertiaTensor(randInertia);
				s = random(s, s * 2);

				break;
			}
		}
	}

	//shape
	int r = random(8);
	PxShape* shape = nullptr;
	if (bType != PLAYERCONTROLLER) {
		if (r > 0)
			shape = CreateShape(PxBoxGeometry(s, s, s));
		else
			shape = CreateShape(PxSphereGeometry(s));
	}
	else
		shape = CreateShape(PxSphereGeometry(s));

	rigid->attachShape(*shape);

	//Actor
	gScene->addActor(*rigid);

	//complete body Struct
	body->rigid = rigid;
	body->isNew = true;
	body->life = lifetime;
	body->force = { 0.f,0.f,0.f };
	body->torque = { 0.f,0.f,0.f };
	body->item = new RenderItem(shape, rigid, color);

	bodies.push_back(body);

	timeSinceAdding = 0;
	numParticles++;
}

void BodySystem::integrate(float t)
{
	timeSinceAdding += t;
	if (timeSinceAdding > step && numParticles < maxParticles && fall != CERO)
		addBody(FALL);
	//Color Player
	if (activeBodyPlayer && bodyPlayer)
		bodyPlayer->item->color = Vector4(1.f,0.8f,0.f, 0.f);
	else if(bodyPlayer)
		bodyPlayer->item->color = Vector4(1.f, 1.f, 1.f, 1.f);

	for (auto it = bodies.begin(); it != bodies.end();) {
		(*it)->rigid->addForce((*it)->force, PxForceMode::eFORCE);
		(*it)->force = Vector3(0);

		(*it)->rigid->addTorque((*it)->torque);
		(*it)->torque = Vector3(0);
		(*it)->life -= t;

		if ((*it)->life <= 0) {
			DeregisterRenderItem((*it)->item);
			delete (*it);
			it = bodies.erase(it);
			numParticles--;
		}

		else
			++it;
	}
}

void BodySystem::setFallGenerator(int type)
{
	if (type > 0 && type < 3) {
		switch (type) {
			case 0:
				fall = CERO;
				break;
			case 1:
				fall = FREE;
				break;
			case 2:
				fall = RAND;
				break;
		}
	}
	else {
		switch (fall)
		{
		case CERO:
			fall = FREE;
			break;
		case FREE:
			fall = RAND;
			break;
		case RAND:
			fall = CERO;
			break;
		}
	}
}

void BodySystem::setCameraPlayer()
{
	activeBodyPlayer = !activeBodyPlayer;
	if (activeBodyPlayer) {
		if (bodyPlayer == nullptr)
			addBody(PLAYERCONTROLLER);

		GetCamera()->lockCam(&bodyPlayer->rigid->getGlobalPose().p);
	}
	else
		GetCamera()->freeCam();

}

void BodySystem::handleInput(unsigned char key)
{
	if (bodyPlayer && activeBodyPlayer) {
		PxVec3 viewX = GetCamera()->getDir().cross(PxVec3(0, 1, 0)).getNormalized();
		PxVec3 viewZ = GetCamera()->getDir().getNormalized();

		float f = 500.f;
		switch (toupper(key))
		{
		case 'W':	bodyPlayer->rigid->addForce(viewZ * f);		break;
		case 'S':	bodyPlayer->rigid->addForce(-viewZ * f);		break;
		case 'A':	bodyPlayer->rigid->addForce(-viewX * f);		break;
		case 'D':	bodyPlayer->rigid->addForce(viewX * f);		break;
		case ' ':	bodyPlayer->rigid->addForce(Vector3(0, 1, 0) * f);		break;

		}
	}
}
