#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(std::vector<Particle*>* part, ParticleForceRegistry* fRegistry) : particles(part), forceRegistry(fRegistry)
{
	srand(time(NULL));
	gravity = new ParticleGravity({ 0,-9.8,0 });
	upward = new ParticleGravity({ 0,20,0 });
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* p : *particles)
		delete p;
	delete gravity;
	delete upward;
}

void ParticleSystem::CreateStaticParticle(float const& vel, float const& acc, float const& size, int const& distance, float lifetime)
{
	Vector3 position, velocity = Vector3(0), acceleration = Vector3(0);
	position = Vector3(GetCamera()->getTransform().p + GetCamera()->getDir() * distance);
	Particle* p = new Particle(position + Vector3(0, -5, 0), velocity, acceleration, 0.4, 30.0f, size, Vector4(1), 0.5);
	particles->push_back(p);
	forceRegistry->add(p, gravity);
}

void ParticleSystem::CreateRandomPosParticle(float const& size, Vector3 min, Vector3 max, float mass, float lifetime)
{
	Vector3 position, velocity = Vector3(0), acceleration = Vector3(0);
	//RANDOM POSITION
	position = Math::random(min, max);
	Particle* p = new Particle(position + Vector3(0, -5, 0), velocity, acceleration, 0.4, 30.0f, size, Vector4(0.2,0.8,0.2,1), 1/mass);
	particles->push_back(p);
	forceRegistry->add(p, gravity);
}

void ParticleSystem::CreateSpring(float const& vel, float const& acc, float const& size, SpringType tipo, int const& distance, float lifetime, float& k, Vector3 pos)
{
	Vector3 position, velocity, acceleration;
	switch (tipo)
	{
	case ANCHORSPRING:
	{
		if (pos.x == 0 && pos.y == 0 && pos.z == 0)
			position = Vector3(GetCamera()->getTransform().p + GetCamera()->getDir() * distance);
		else
			position = pos;
		velocity = Vector3(0);
		acceleration = Vector3(0);
		Particle* p = new Particle(position + Vector3(0, -0.1, 0), velocity, acceleration, 0.4, 30.0f, size, { 1, 0, 0, 1 }, 0.5);
		particles->push_back(p);
		forceRegistry->add(p, gravity);

		ParticleAnchoredSpring* anchorSp = new ParticleAnchoredSpring(position, k, 3.5f);
		forceRegistry->add(p, anchorSp);
		break;
	}
	case DOUBLESPRING:
	{
		if (pos.x == 0 && pos.y == 0 && pos.z == 0)
			position = Vector3(GetCamera()->getTransform().p + GetCamera()->getDir() * distance);
		else
			position = pos;
		velocity = Vector3(0);
		acceleration = Vector3(0);
		Particle* p1 = new Particle(position + Vector3(1*size), velocity, acceleration, 0.4, lifetime, size, { 1, 0, 0, 1 }, 0.5);
		particles->push_back(p1);
		Particle* p2 = new Particle(position + Vector3(-1*size), velocity, acceleration, 0.4, lifetime, size, { 0, 0, 1, 1 }, 0.5);
		particles->push_back(p2);

		ParticleSpring* pS1 = new ParticleSpring(p2, k, 3.5f);
		ParticleSpring* pS2 = new ParticleSpring(p1, k, 3.5f);
		forceRegistry->add(p1, pS1);
		forceRegistry->add(p2, pS2);
		break;
	}
	}
}

void ParticleSystem::CreateFw(Vector3 pos, Vector3 vel, Vector3 accel, float size, Fireworks tipo)
{
	fw_type = tipo;
	Vector4 c;
	switch (fw_type)
		{
			case EXPLOSION_: {
				c = { 0.8, 0.2, 0, 1 };
				break; 
				}
			case STAR_: {
				c = { 0.8, 0.8, 0.2, 1 };
				break; 
				}
			case NOVA_:{
				c = { 0.4, 0.2, 0.8, 1 };
				break;
				}
		}

	Particle* p = new Particle(pos, vel, Vector3(0), 0.99, fw_payload_time, size, c, 1, true);
	payload = p;
	particles->push_back(p);
	fw_timer = timer_ + fw_payload_time;
}

void ParticleSystem::DetonateFw(Vector3 pos)
{
	Vector3 velocity, acceleration;

	acceleration = Vector3(0);

	int nParticles = 50;

	Vector4 col;

	switch (fw_type)
	{
		case EXPLOSION_: {
			nParticles = 360;
			float ang = -90;
			for (int i = 0; i < nParticles; ++i) {
				float force = random(10, 25);
				col = Vector4(Vector3(random(Vector3(0), Vector3(10)) / 10.f), 1.f);
				velocity = Vector3(cos(radians(ang*(random(5,10)/10.f))) * random(-3, 3) * force, sin(radians(ang*(random(5, 10) / 10.f))) * random(-3, 3)*force, random(-3, 3)*force*0.3);
				Particle* p = new Particle(pos, velocity, acceleration, 0.8, 4.0f, 0.3, col, 0.0002f);
				particles->push_back(p);
				ang += 360 / 180;
			}
			break;
		}
		case STAR_: {
			for (int i = 0; i < nParticles; ++i) {
				hsv col = { 45.0f + rand() % 15, 0.8f + (rand() % 3) / 10.0f, 0.4f + (rand() % 7) / 10.0f };
				float ang = 360.0f / nParticles;	// angle between each particle trajectory
				float currentAng = ang * i;			// current particle's angle relative to world's X axis

				velocity = {
					(float)cos(currentAng) * 20.0f + 10.0f,
					(float)sin(currentAng) * 20.0f,
					0 };
				particles->push_back(new Particle(pos, velocity, acceleration, 0.4, 5.f, (5.0f + rand() % 10) / 10.0f * 0.3, { hsv2rgb(col).r, hsv2rgb(col).g, hsv2rgb(col).b, 1 }, 0.5));
			}
			for (int i = 0; i < nParticles; i++) {
				hsv col = { 45.0f + rand() % 15, 0.8f + (rand() % 3) / 10.0f, 0.4f + (rand() % 7) / 10.0f };
				float ang = 360.0f / nParticles;	// angle between each particle trajectory
				float currentAng = ang * i;			// current particle's angle relative to world's X axis

				velocity = {
					(float)cos(currentAng) * 20.0f - 10.0f,
					(float)sin(currentAng) * 20.0f,
					0 };
				particles->push_back(new Particle(pos, velocity, acceleration, 0.4, 5.f, (5.0f + rand() % 10) / 10.0f * 0.3, { hsv2rgb(col).r, hsv2rgb(col).g, hsv2rgb(col).b, 1 }, 0.5));
			}
			for (int i = 0; i < nParticles; i++) {
				hsv col = { 45.0f + rand() % 15, 0.8f + (rand() % 3) / 10.0f, 0.4f + (rand() % 7) / 10.0f };
				float ang = 360.0f / nParticles;	// angle between each particle trajectory
				float currentAng = ang * i;			// current particle's angle relative to world's X axis

				velocity = {
					(float)cos(currentAng) * 20.0f,
					(float)sin(currentAng) * 20.0f + 10.0f,
					0 };
				particles->push_back(new Particle(pos, velocity, acceleration, 0.4, 5.0f, (5.0f + rand() % 10) / 10.0f * 0.3, { hsv2rgb(col).r, hsv2rgb(col).g, hsv2rgb(col).b, 1 }, 0.5));
			}
			for (int i = 0; i < nParticles; i++) {
				hsv col = { 45.0f + rand() % 15, 0.8f + (rand() % 3) / 10.0f, 0.4f + (rand() % 7) / 10.0f };
				float ang = 360.0f / nParticles;	// angle between each particle trajectory
				float currentAng = ang * i;			// current particle's angle relative to world's X axis

				velocity = {
					(float)cos(currentAng) * 20.0f,
					(float)sin(currentAng) * 20.0f - 10.0f,
					0 };
				particles->push_back(new Particle(pos, velocity, acceleration, 0.4, 5.f, (5.0f + rand() % 10) / 10.0f * 0.3, { hsv2rgb(col).r, hsv2rgb(col).g, hsv2rgb(col).b, 1 }, 0.5));
			}
			diamond(pos, 10.0f, acceleration, 0.7, 5.0f, 0.2, 20, 20);
			break;
		}
		case NOVA_: {
			for (int i = 0; i < nParticles; ++i) {
				float ang = 360.0f / nParticles;	// angle between each particle trajectory
				float currentAng = ang * i;			// current particle's angle relative to world's X axis

				velocity = {
					(float)cos(currentAng) * 30.0f,
					(float)sin(currentAng) * 30.0f,
					0 };

				hsv col = { 170.0f + rand() % 25, 0.6f + (rand() % 5) / 10.0f, 1.0f };
				particles->push_back(new Particle(pos, velocity, acceleration, 0.7, 5.f, 0.3, { hsv2rgb(col).r, hsv2rgb(col).g, hsv2rgb(col).b, 1 }, 0.5));
			}
			for (int i = 0; i < nParticles / 2; ++i) {
				velocity = {
					(float)cos((360.0f / (nParticles / 2)) * i) * 25.0f,
					(float)sin((360.0f / (nParticles / 2)) * i) * 25.0f,
					0 };

				hsv col = { 190.0f + rand() % 25, 0.6f + (rand() % 5) / 10.0f, 1.0f };
				particles->push_back(new Particle(pos, velocity, acceleration, 0.7, 5.f, 0.3, { hsv2rgb(col).r, hsv2rgb(col).g, hsv2rgb(col).b, 1 }, 0.5));
			}
			sphere(pos, 10.0f, acceleration, 0.8, 5.0f, 0.3, 20, 20);
			explosion(pos, 10.0f, acceleration, 0.8, 5.0f, 0.3, 20, 20);
			break;
		}
	}
}

void ParticleSystem::update(double t)
{
	if (payload != nullptr) {
		Particle* p = new Particle(payload->getPosition(), Vector3(random(-3, 3), random(-3, 3), random(2, 40)), Vector3(0), 0.9, 0.5f, 0.3f, Vector4(0.8, 0.7,0,1.), 1 / 0.1);
		particles->push_back(p);
	}
	for (auto it = particles->begin(); it != particles->end();)
	{
		(*it)->update(t);
		if ((*it)->dead()) {
			/*if ((*it)->IsPayload())
				DetonateFw((*it)->getPos().p);*/
			if(forceRegistry != nullptr) forceRegistry->clearParticle((*it));
			delete (*it);
			it = particles->erase(it);
		}
			else
				++it;
	}

}

void ParticleSystem::explosion(Vector3 pos, float const& vel, Vector3 const& accel,
	const double& damp, const float& time, float const& size,
	int const& segments, int const& rings)
{
	Vector3 velocity;

	hsv col;

	// Angle between rings
	float angRings = 180.0 / rings;
	// Angle between segments
	float angSegments = 360.0 / segments;

	for (int i = 0; i < rings; i++) {
		//! Generate ring nº i

		// Normalized ring i latitude
		float latRing = -cos(angRings * i);

		for (int j = 0; j < segments; j++) {
			//! Generate segment j of ring i

			// Segment j of ring i angle
			float angCurrentSeg = angSegments * j;

			velocity = { latRing / (float)cos(angCurrentSeg) * vel,
				latRing * vel,
				latRing / (float)sin(angCurrentSeg) * vel };

			col = { 290.0f + rand() % 5, 0.8f + (rand() % 3) / 10.0f, 0.4f + (rand() % 7) / 10.0f };
			rgb rgb = hsv2rgb(col);

			particles->push_back(new Particle(pos, velocity, accel, damp, time, size, { rgb.r, rgb.g, rgb.b, 1 }, 0.5));
		}
	}
}

void ParticleSystem::diamond(Vector3 pos, float const& vel, Vector3 const& accel,
	const double& damp, const float& time, float const& size,
	int const& segments, int const& rings)
{
	Vector3 velocity;

	hsv col;

	// Angle between rings
	float angRings = 180.0 / rings;
	// Angle between segments
	float angSegments = 360.0 / segments;

	for (int i = 0; i < rings; i++) {
		//! Generate ring nº i

		// Normalized ring i latitude
		float latRing = -cos(angRings * i);

		for (int j = 0; j < segments; j++) {
			//! Generate segment j of ring i

			// Segment j of ring i angle
			float angCurrentSeg = angSegments * j;

			velocity = {
				((float)cos(angCurrentSeg) - abs(latRing) * (float)cos(angCurrentSeg)) * vel,
				latRing * vel,
				((float)sin(angCurrentSeg) - abs(latRing) * (float)sin(angCurrentSeg)) * vel };

			col = { 100.0f + rand() % 5, 0.8f + (rand() % 3) / 10.0f, 0.4f + (rand() % 7) / 10.0f };
			rgb rgb = hsv2rgb(col);

			particles->push_back(new Particle(pos, velocity, accel, damp, time, (5.0f + rand() % 10) / 10.0f * size, { rgb.r, rgb.g, rgb.b, 1 }, 0.5));
		}
	}
}

void ParticleSystem::sphere(Vector3 pos, float const& vel, Vector3 const& accel,
	const double& damp, const float& time, float const& size,
	int const& segments, int const& rings)
{
	Vector3 velocity;

	hsv col;

	// Angle between rings
	float angRings = radians(180.0) / rings;
	// Angle between segments
	float angSegments = radians(360.0) / segments;

	for (int i = 0; i < rings; i++) {
		//! Generate ring nº i

		// ring i latitude
		float latRing = radians(90.0);
		latRing += angRings * i;

		// ring i height
		float ringHeight = sin(latRing);

		// Radius of ring i
		float radCurrentRing = cos(latRing);

		for (int j = 0; j < segments; j++) {
			//! Generate segment j of ring i

			// Segment j of ring i angle
			float angCurrentSeg = angSegments * j;

			velocity = {
				(float)cos(angCurrentSeg) * radCurrentRing * vel,
				ringHeight * vel,
				(float)sin(angCurrentSeg) * radCurrentRing * vel };

			col = { 200.0f + rand() % 90, 0.8f, 1.0f };

			particles->push_back(new Particle(pos, velocity, accel, damp, time, (5.0f + rand() % 10) / 10.0f * size, { hsv2rgb(col).r,  hsv2rgb(col).g,  hsv2rgb(col).b, 1 }, 0.5));
		}
	}
}
