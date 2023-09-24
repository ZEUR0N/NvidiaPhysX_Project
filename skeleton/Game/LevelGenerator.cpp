#include "LevelGenerator.h"

LevelGenerator::LevelGenerator(PxPhysics* _gPhysics, PxScene* _gScene): gPhysics(_gPhysics), gScene(_gScene)
{
	mat = _gPhysics->createMaterial(0.f, 0.f, 1.f);
	
	initStage();
}

LevelGenerator::~LevelGenerator()
{
	delete forceRegistry, particleSys, fieldSys;
	delete fireworks_particleSys, fireworks2_particleSys;
	delete spectatorsLeft_bodySys, spectatorsRight_bodySys;
	for (StaticPlatforms* plat : platforms) {
		delete plat;
	}
	for (RenderItem* i : items) {
		DeregisterRenderItem(i);
		delete i;
	}
	for (Particle* p : *particles)
		delete p;
	for (Particle* fw_p : *fw_particles)
		delete fw_p;
}

void LevelGenerator::update(float t)
{
	timer_ += t;
	checkTimers();
//Platforms
	for (auto it = platforms.begin(); it != platforms.end();) {
		if ((*it)->rigid->getGlobalPose().p.z > zDeathLocation) {
			DeregisterRenderItem((*it)->item);
			delete (*it);
			it = platforms.erase(it);
		}
		else {
			(*it)->rigid->setGlobalPose(PxTransform((*it)->rigid->getGlobalPose().p + Vector3(0, 0, zMoveSpeed*t)));
			++it;
		}
	}
//Particles & Bodies
	if (activeForces) {
		forceRegistry->addNews(spectatorsLeft_bodySys, { windForce });
		forceRegistry->addNews(spectatorsRight_bodySys, { windForce, torqueGenerator });
	}
	forceRegistry->updateForces(t);
	particleSys->update(t);
	fireworks_particleSys->update(t);
	fireworks2_particleSys->update(t);
	fieldSys->update(t);
	spectatorsLeft_bodySys->integrate(t); 
	spectatorsRight_bodySys->integrate(t);
	forceRegistry->clearBody(spectatorsLeft_bodySys);
	forceRegistry->clearBody(spectatorsRight_bodySys);

}

void LevelGenerator::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
		case 'G':
		{
			if (!started) startGame();
			break;
		}
		case '0':
		{
			activeForces = !activeForces;
			break;
		}
		case '1':
		{
			fieldSys->createSphereForceField(30.0, 20, GRAVITYWELL, Vector3(0, 50, zlocationSpawn - 20));
			break;
		}
		case '2':
		{
			fieldSys->createSphereForceField(75.0, 20, WHIRLWIND, Vector3(0, 50, zlocationSpawn - 20));
			break;
		}
		case '3':
		{
			fieldSys->createSphereForceField(30.0, 20, EXPLOSION, Vector3(0, 50, zlocationSpawn - 20));
			break;
		}
		case 'U':
		{
			spectatorsLeft_bodySys->setFallGenerator();
			spectatorsRight_bodySys->setFallGenerator(2);
			break;
		}
		case 'I':
		{
			particleSys->CreateSpring(0, 0, 2.f, DOUBLESPRING, 20.0f, 30000.0f, k, Vector3(0, 50, zlocationSpawn - 20));
			break;
		}
		case 'O':
		{
			bouyanceExample = !bouyanceExample;
			if(bouyanceExample)
				for (int i = 0; i < 20; ++i)
					createNewParticle();
			break;
		}
		case 'P':
		{
			//createFireWorks();
			fireworks_particleSys->DetonateFw(Vector3(xLocationSpectators[0] * 1.5, 40, zlocationSpectators * 2));
			fireworks2_particleSys->DetonateFw(Vector3(xLocationSpectators[1] * 1.5, 40, zlocationSpectators * 2));
			break;
		}
		case 'K':
		{
			fireworks_particleSys->DetonateFw(Vector3(xLocationSpectators[0], 40, 20 + (-50 * 4)));
			fireworks2_particleSys->DetonateFw(Vector3(xLocationSpectators[1], 40, 20 + (-50 * 4)));
			break;
		}
	}
}

void LevelGenerator::initStage()
{
//Static Platforms
	PxShape* shape = CreateShape(PxBoxGeometry(30, 1, 100));
	PxRigidStatic* ground1 = gPhysics->createRigidStatic({ xLocationSpectators[0], 10, -50 });
	ground1->attachShape(*shape);
	gScene->addActor(*ground1);
	RenderItem* item1 = new RenderItem(shape, ground1, { 0.2,1,0.6,1 });
	items.push_back(item1);
	PxRigidStatic* ground2 = gPhysics->createRigidStatic({ xLocationSpectators[1], 10, -50 });
	ground2->attachShape(*shape);
	gScene->addActor(*ground2);
	RenderItem* item2 = new RenderItem(shape, ground2, { 0.2,1,0.6,1 });
	items.push_back(item2);
	PxShape* shape2 = CreateShape(PxBoxGeometry(160, 1, 100));
	PxRigidStatic* groundDeath = gPhysics->createRigidStatic({ 0, -25, -50 });
	groundDeath->attachShape(*shape2);
	gScene->addActor(*groundDeath);
	RenderItem* item3 = new RenderItem(shape2, groundDeath, { 1,0.2,0.2,0 });
	items.push_back(item3);

//Particles & Bodies
	//sistema de solido rigidos
	spectatorsLeft_bodySys = new BodySystem(gPhysics, gScene, { xLocationSpectators[0],20,zlocationSpectators }, 0.3, false, 4.0f, 0.8, 100, Vector4(1), 0.1);
	spectatorsRight_bodySys = new BodySystem(gPhysics, gScene, { xLocationSpectators[1],40, zlocationSpectators }, 1.f, true, 12.f);

	particles = new std::vector<Particle*>();
	fw_particles = new std::vector<Particle*>();
	//generadores de fuerzas y torques
	windForce = new BodyWind({ 0.0f,0.0f,50.f });
	torqueGenerator = new BodyTorque({ 40.0f,-200.0f,60.0f });

	forceRegistry = new ParticleForceRegistry();

	particleSys = new ParticleSystem(particles, forceRegistry);
	fireworks_particleSys = new ParticleSystem(fw_particles, nullptr);
	fireworks2_particleSys = new ParticleSystem(fw_particles, nullptr);

	fieldSys = new FieldSystem(particles, spectatorsLeft_bodySys, forceRegistry);

	particleSys->CreateSpring(0, 0, 2.f, DOUBLESPRING, 20.0f, 30000.0f, k, Vector3(0, 50, zlocationSpawn - 20));
}

void LevelGenerator::createPlatform(Vector3 pos)
{
	StaticPlatforms* plat = new StaticPlatforms;
	plat->rigid = gPhysics->createRigidDynamic(PxTransform(pos));
	plat->rigid->createShape(PxBoxGeometry(plat_size), *mat);
	////making Kinematic
	plat->rigid->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	plat->rigid->setMass(0.f);
	plat->rigid->setMassSpaceInertiaTensor(PxVec3(0.f));
	gScene->addActor(*plat->rigid);
	plat->shape = CreateShape(PxBoxGeometry(plat_size));
	plat->item = new RenderItem(plat->shape, plat->rigid, { 0.6,0.2,1,1 });

	platforms.push_back(plat);
	items.push_back(plat->item);
}

void LevelGenerator::createNewParticle()
{
	float s = random(1, 3);
	float mass = random(1, 7);
	particleSys->CreateRandomPosParticle(s, Vector3(-80, -10, -80), Vector3(80, -5, 10), mass);
}

void LevelGenerator::createFireWorks()
{
	int r1 = random(0, 3);
	int r2 = random(0, 3);
	Fireworks t1, t2;
	if (r1 == 0)
		t1 = EXPLOSION_;
	else if (r1 == 1)
		t1 = STAR_;
	else if (r1 == 2)
		t1 = NOVA_;

	if (r2 == 0)
		t2 = EXPLOSION_;
	else if (r2 == 1)
		t2 = STAR_;
	else if (r2 == 2)
		t2 = NOVA_;

	Vector3 vel = Vector3(0, 0, -50);
	Vector3 acc = Vector3(0, 0, -50);

	fireworks_particleSys->CreateFw(Vector3(xLocationSpectators[0], 40, 20), vel, acc, 0.4, t1);
	fireworks2_particleSys->CreateFw(Vector3(xLocationSpectators[1], 40, 20), vel, acc, 0.4, t2);
	launched = true;
}

void LevelGenerator::checkTimers()
{
	if (timer_ >= fw_timer) {
		createFireWorks();
		fw_timer = timer_ + fireWorks_counter;
	}
	if (timer_ >= bouyance_timer) {
		fieldSys->createBoxForceField(Vector3(80, 20, 80), 40.0f, BOUYANCE, Vector3(0, -40, -50));
		bouyance_timer = timer_ + bouyance_counter;
	}
	if (launched && timer_ >= fw_timer - fireWorks_counter + 2.f) {
		fireworks_particleSys->DetonateFw(Vector3(xLocationSpectators[0], 40, 20 + (-50 * 4)));
		fireworks2_particleSys->DetonateFw(Vector3(xLocationSpectators[1], 40, 20 + (-50 * 4)));
		launched = false;
	}
	if (started && timer_ >= plat_timer) {
		int r1 = random(0,2), r2 = random(0,2), r3 = random(0, 2);
		if(r1)createPlatform(Vector3(xLocationSpawns[0], 0, zlocationSpawn));
		if(!r1 && !r2 && !r3)createPlatform(Vector3(xLocationSpawns[1], 0, zlocationSpawn));
		else if(r2)createPlatform(Vector3(xLocationSpawns[1], 0, zlocationSpawn));
		if(r3)createPlatform(Vector3(xLocationSpawns[2], 0, zlocationSpawn));
		plat_timer = timer_ + platforms_counter;
	}
	if (timer_ >= particles_timer && bouyanceExample) {
		if (particles->size() < maxParticles)
			createNewParticle();
		particles_timer = timer_ + particles_counter;
	}
}

void LevelGenerator::startGame()
{
	//Kinematic Platforms
	createPlatform(Vector3(xLocationSpawns[0], 0, zlocationSpawn / 2.5));
	createPlatform(Vector3(xLocationSpawns[1], 0, zlocationSpawn / 2.5));
	createPlatform(Vector3(xLocationSpawns[2], 0, zlocationSpawn / 2.5));
	plat_timer = timer_ + 0.4f;
	started = true;
}
