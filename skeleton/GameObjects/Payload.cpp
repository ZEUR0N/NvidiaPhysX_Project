#include "Payload.h"
#include <stdlib.h>
#include <time.h>


Payload::Payload(physx::PxShape* sh, physx::PxTransform tr, Vector4 color, Vector3 vel, Vector3 acc) : ParticleSystem(sh, tr, color, vel, acc)
{
	duration = 6.0f;
	min_lifeTime = 0.5f; max_lifetime = 1.0f;
	iniParticles = 1; 
	maxParticles = 100;
	end_scale = 0.05;
	init();

}

void Payload::update(float t)
{
	randomValues();
	ParticleSystem::update(t);
}

physx::PxTransform Payload::getTransform() const
{
	return ParticleSystem::getTransform();
}

void Payload::randomValues()
{
	//Lifetime
	int rand_lf = (rand() % ((int)(max_lifetime - min_lifeTime + 1) * 10)) + min_lifeTime * 10;	//multiplo de diez para decimales
	lifetime = rand_lf / 10.0f;	//Conversion a decimales de nuevo

	//Velocidad
	int vx = rand() % 3 + (-1);	//21 pq hay 21 numeros entre -10 y 10 inclusives
	int vy = rand() % 3 + (-1);
	int vz = rand() % 3 + (-1);
	start_speed = Vector3(vx, vel_.y, vz) / 10;

	//Aceleracion
	int ax = rand() % 3 + (-1);
	int az = rand() % 3 + (-1);
	acceleration = Vector3(ax, GRAVITY.y, az) / 10;
}


