#pragma once
#include "../Forces/FieldSystem.h"
#include "../Forces/BodyForces.h"




enum RoadType{PLATFORM, WINDFORCE, BLACKHOLE};

struct StaticPlatforms {
	PxRigidDynamic* rigid;
	RenderItem* item;
	PxShape* shape;
};

class LevelGenerator {
public:
	LevelGenerator(PxPhysics* _gPhysics, PxScene* _gScene);
	~LevelGenerator();
	void update(float t);
	void handleInput(unsigned char key);

private:

	void initStage();
	void createPlatform(Vector3 pos);
	void createNewParticle();
	void createFireWorks();
	void checkTimers();	//Comprobacion de los timers
	void startGame();

	PxPhysics* gPhysics;
	PxScene* gScene;
	PxMaterial* mat;

	BodySystem* spectatorsLeft_bodySys = nullptr;
	BodySystem* spectatorsRight_bodySys = nullptr;

	ParticleSystem* particleSys;

	ParticleSystem* fireworks_particleSys;
	ParticleSystem* fireworks2_particleSys;

	FieldSystem* fieldSys;

	std::vector<Particle*>* particles;
	std::vector<Particle*>* fw_particles;

	ParticleForceRegistry* forceRegistry;

	BodyWind* windForce = nullptr;
	BodyTorque* torqueGenerator = nullptr;

	vector<StaticPlatforms*> platforms;
	vector<RenderItem*> items;

	//Parameters
	float fireWorks_counter = 15, bouyance_counter = 30, platforms_counter = 1.95f, particles_counter = 0.2;
	float timer_ = 0, fw_timer = 15, bouyance_timer = 20, plat_timer = 0.4f, particles_timer = particles_counter;
	Vector3 plat_size = Vector3(12, 1, 12);
	float zMoveSpeed = 40;
	int maxParticles = 100;
	float k = 5.0f;

	bool started = false;
	bool bouyanceExample = false;
	bool launched = false;
	bool activeForces = false;
	//Constants
	const float xLocationSpawns[3] = { -40,0,40 };
	const float zlocationSpawn = -100;

	const float xLocationSpectators[2] = { -100, 100 };
	const float zlocationSpectators = -80;

	const float zDeathLocation = 20;
};