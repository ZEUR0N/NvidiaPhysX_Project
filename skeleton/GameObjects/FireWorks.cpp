#include "FireWorks.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>


FireWorks::FireWorks(physx::PxShape* sh, physx::PxTransform tr, Vector3 vel, Vector3 acc, int fwtype) : ParticleSystem(sh, tr, {1,0,0,1}, vel, acc)
{
	aux_dur = duration = 8.0f;
	min_lifeTime = 3.0f; max_lifetime = duration;
	iniParticles = maxParticles = 250;
	capOnMaxP = true;
	end_scale = 0.05;
	randomColors = false, updateColor = false;
	ang = -90;

	fwType = fwtype;	//tipo de firework

	srand(time(NULL));	//random seed para los aleatorios
	loadChar();		//Caracter˙êticas predefinidas de la explosion
	explosion();	//Tipo de explosion
}

void FireWorks::update(float t)
{
	ParticleSystem::update(t);
	//**Degradado de particula ROJA ORIGINAL
	if (!randomColors && color_.y < 0.8f) color_.y += 0.0005f;
	//Cambiar color de la particula durante su vida
	if (updateColor) 
		for (Particle* p : *particles)
			p->setColor(color_);
	//Crear multiples explosiones hasta que se alcance el limite
	if (particles->size() < maxParticles && !reachedCap)
		explosion();
	//Reiniciar la explosion si se ha acabado
	if (duration<=0 && loop) {
		explosion();
		duration = aux_dur;
		reachedCap = false;
	}
}

void FireWorks::loadChar()
{
	switch (fwType)
	{
	case 0:
		charType1();
		break;
	case 1:
		charType2();
		break;
	case 2:
		charType3();
		break;
	}
}

void FireWorks::explosion()
{
	switch (fwType)
	{
	case 0:
		type1();
		break;
	case 1:
		type2();
		break;
	case 2:
		type3();
		break;
	}
}

void FireWorks::charType1()
{
	//Caracer˙êticas predefinidas
	color_ = Vector4(1, 0, 0, 1);
	updateColor = true;
	aux_dur = duration = 6.0f;
	max_lifetime = duration;
}

void FireWorks::type1()
{
	//Creacion de las particulas de la explosion
	for (int i = 0; i < maxParticles; ++i) {
	//Lifetime
	int rand_lf = (rand() % ((int)(max_lifetime - min_lifeTime + 1) * 10)) + min_lifeTime * 10;	//multiplo de diez para decimales
	lifetime = rand_lf / 10.0f;	//Conversion a decimales de nuevo

	//Velocidad
	int vx = rand() % 21 + (-10);	//21 pq hay 21 numeros entre -10 y 10 inclusives
	int vy = rand() % 21 + (-10);
	int vz = rand() % 11 + (-5);	//-0.5 a 0.5
	start_speed = (Vector3(vx, vy, vz) / 10) * 2;

	//Aceleracion constante pero mas lento
	acceleration = (Vector3(vx, vy, vz) / 10) / 20;

	//Creacion de la particula
	createNewParticle();
	}
}

void FireWorks::charType2()
{
	//Caracer˙êticas predefinidas
	loop = true;
	randomColors = true;
}

void FireWorks::type2()
{
	//Color
	randomColor();
	//Creacion de las particulas de la explosion
	for (int i = 0; i < maxParticles; ++i) {
		//Lifetime
		int rand_lf = (rand() % ((int)(max_lifetime - min_lifeTime + 1) * 10)) + min_lifeTime * 10;	//multiplo de diez para decimales
		lifetime = rand_lf / 10.0f;	//Conversion a decimales de nuevo
		//Velocidad
		int v = rand() % 3 + 6;	//Solo queremos entre 0.6 y 0.8
		int vz = rand() % 3 - 2;	///-0.2 a 0.2
		//Reposicionamiento
		start_speed = (Vector3(cos(radians(ang)) * v, sin(radians(ang)) * v,vz) / 10);

		//Aceleracion constante pero mas lento
		acceleration = Vector3(start_speed.x, start_speed.y, start_speed.z) / 25;
			ang += 360.0 / maxParticles;
		//Creacion de la particula
		createNewParticle();
	}
}

void FireWorks::charType3()
{
	//Caracer˙êticas predefinidas
	loop = true;
	aux_dur = duration = 0.0f;
	min_lifeTime = 0.5f;
	max_lifetime = 3.0f;
	randomColors = true;
	iniParticles = 1;
	color_ = Vector4(1, 1, 1, 1);
}

void FireWorks::type3()
{
	//Color
	randomColor(true, false, true);
	//Lifetime
	int rand_lf = (rand() % ((int)(max_lifetime - min_lifeTime + 1) * 10)) + min_lifeTime * 10;	//multiplo de diez para decimales
	lifetime = rand_lf / 10.0f;	//Conversion a decimales de nuevo
	//Velocidad
	int v = rand() %  3 + 8;	//Solo queremos entre 0.8 y 1.0
	int vz = rand() % 3 - 2;	///-0.2 a 0.2
	//Reposicionamiento
	start_speed = (Vector3(cos(radians(ang)) * v, sin(radians(ang)) * v, vz) / 10);

	//Aceleracion constante pero mas lento
	acceleration = Vector3(start_speed.x, start_speed.y, start_speed.z) / 5;
	ang += 1440.0 / maxParticles;
	//Creacion de la particula
	createNewParticle();
}

void FireWorks::randomColor(bool x, bool y, bool z)
{
	if (randomColors) {
		int cx = color_.x * 10, cy = color_.y * 10, cz = color_.z * 10;
		if(x)cx = rand() % 9 + 2;	//entre 2 y 10
		if(y)cy = rand() % 9 + 2;	//entre 2 y 10
		if(z)cz = rand() % 9 + 2;	//entre 2 y 10
		color_ = Vector4(cx, cy, cz, 10) / 10;
	}
}
