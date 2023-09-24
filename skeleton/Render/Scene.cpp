#include "Scene.h"

Scene::Scene(PxPhysics* _gPhysics, PxScene* _gScene)
{
	//InitPhysics
	gPhysics = _gPhysics;
	gScene = _gScene;

	//Custom
	//sistema de solido rigidos
	bodySys = new BodySystem(gPhysics, gScene, { 0,40,0 }, 0.8, true, 20.f);

	particles = new std::vector<Particle*>();
	//generadores de fuerzas y torques
	windForce = new BodyWind({ 0.0f,2.0f,0.0f });
	torqueGenerator = new BodyTorque({ -100.0f,20.0f,0.0f });

	forceRegistry = new ParticleForceRegistry(); 

	particleSys = new ParticleSystem(particles,forceRegistry);
	fieldSys = new FieldSystem(particles, bodySys, forceRegistry);
}

Scene::~Scene()
{
	if(anchor != nullptr) DeregisterRenderItem(anchor);
	delete forceRegistry, particleSys, fieldSys, anchor;
}

void Scene::update(const double& t)
{
	if (bodySys) {
		if(activeForces)forceRegistry->addNews(bodySys, { windForce, torqueGenerator });
		forceRegistry->updateForces(t);
		particleSys->update(t);
		fieldSys->update(t);
		bodySys->integrate(t);
		forceRegistry->clearBody(bodySys);
	}
	GetCamera()->updatePos();
}

void Scene::keyPress(unsigned char key)
{
	bodySys->handleInput(key);
	switch (toupper(key))
	{
	case 'Z':
	{
		activeForces = !activeForces;
		break;
	}
	case '0':
	{
		fieldSys->createSphereForceField(10.0, 20, GRAVITYWELL);
		break;
	}
	case '1':
	{
		fieldSys->createSphereForceField(10.0, 20, WIND);
		break;
	}
	case '2':
	{
		fieldSys->createSphereForceField(20.0, 20, WHIRLWIND);
		break;
	}
	case '3':
	{
		fieldSys->createSphereForceField(7.0, 20, EXPLOSION);
		break;
	}
	case '4':
	{
		fieldSys->createBoxForceField(Vector3(20, 10, 20), 40.0f, BOUYANCE);
		break;
	}
	case 'P': {
		bodySys->setCameraPlayer();
		break;
	}
	case 'E':{
		bodySys->addBody(BULLET, GetCamera()->getDir() * 80.f);
		break;
	}
	case 'F': {
		bodySys->setFallGenerator();
		break;
	}
	case 'J':  //Spring Anchor
	{
		Vector3 pos_ = Vector3(GetCamera()->getTransform().p + GetCamera()->getDir() * 20.0f);
		physx::PxShape* sh = CreateShape(physx::PxBoxGeometry(0.1f, 0.1f, 0.1f));
		anchor = new RenderItem(sh, &physx::PxTransform(pos_), Vector4(1));
		particleSys->CreateSpring(0, 0, 0.3f, ANCHORSPRING, 20.0f, 30.0f, k);
		break; 
	}
	case 'K':	//Spring particles
	{
		particleSys->CreateSpring(0, 0, 0.3f, DOUBLESPRING, 20.0f, 30.0f, k);
		break;
	}
	case 'L':	//Float
	{
		particleSys->CreateStaticParticle(0, 0, 0.3f, 40.0f);
		break;
	}
	case 'B':
	{
		//Box
		PxMaterial* mMaterial;
		mMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);    //static friction, dynamic friction, restitution

		float size = 50;
		PxShape* shape = CreateShape(PxBoxGeometry(size, 1, size));
		PxRigidStatic* ground = gPhysics->createRigidStatic({ 0,0,0 });
		ground->attachShape(*shape);
		gScene->addActor(*ground);
		RenderItem* item = nullptr;
		item = new RenderItem(shape, ground, { 0.6,0.2,1,1 });

		PxShape* shape1 = CreateShape(PxBoxGeometry(size, size, 1));
		PxRigidStatic* back = gPhysics->createRigidStatic({ 0,size,-size });
		back->attachShape(*shape1);
		gScene->addActor(*back);
		RenderItem* item1 = nullptr;
		item1 = new RenderItem(shape1, back, { 0.6,0.2,1,1 });

		PxShape* shape2 = CreateShape(PxBoxGeometry(1, size, size));
		PxRigidStatic* right = gPhysics->createRigidStatic({ size,size,0 });
		right->attachShape(*shape2);
		gScene->addActor(*right);
		RenderItem* item2 = nullptr;
		item2 = new RenderItem(shape2, right, { 0.6,0.2,1,1 });

		PxShape* shape3 = CreateShape(PxBoxGeometry(1, size, size));
		PxRigidStatic* left = gPhysics->createRigidStatic({ -size,size,0 });
		left->attachShape(*shape3);
		gScene->addActor(*left);
		RenderItem* item3 = nullptr;
		item3 = new RenderItem(shape3, left, { 0.6,0.2,1,0.f });

		PxShape* shape4 = CreateShape(PxBoxGeometry(size, size, 1));
		PxRigidStatic* front = gPhysics->createRigidStatic({ 0,size,size });
		front->attachShape(*shape4);
		gScene->addActor(*front);
		RenderItem* item4 = nullptr;
		item4 = new RenderItem(shape4, front, { 0.6,0.2,1,0.f });
		break;
	}
	case '+':
	{
		k++;
		break;
	}
	case '-':
	{
		if(k>1.0f)k--;
		break;
	}
	default:
		break;
	}
}