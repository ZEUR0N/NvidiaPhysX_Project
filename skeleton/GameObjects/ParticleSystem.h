#pragma once
#include <math.h>

#include "Particle.h"
#include "../utils/Utils.h"
#include "../utils/Color.h"

#include "../Forces/ParticleForces.h"
#include "../Forces/ParticleForceRegistry.h"

using namespace Math;

enum Fireworks {
	EXPLOSION_, STAR_, NOVA_
};

enum Source {
	UPWARD, FIRE, SPROUT, SWARM, STATIC
};
enum SpringType {
	ANCHORSPRING, DOUBLESPRING
};

class ParticleSystem
{
public:
	ParticleSystem(std::vector<Particle*>* particles, ParticleForceRegistry* fRegistry);
	~ParticleSystem();
	//Creation
	void CreateStaticParticle(float const& vel, float const& acc, float const& size, int const& distance, float lifetime = 30.0f);
	void CreateRandomPosParticle (float const& size, Vector3 min, Vector3 max, float mass, float lifetime = 25.0f);
	void CreateSpring(float const& vel, float const& acc, float const& size, SpringType tipo, int const& distance, float lifetime ,float& k, Vector3 pos = Vector3(0));
	void CreateFw(Vector3 pos, Vector3 vel, Vector3 accel, float size, Fireworks tipo);

	void update(double t);
	void DetonateFw(Vector3 pos);

private:
	void explosion(Vector3 pos, float const& vel, Vector3 const& accel, const double& damp, const float& time, float const& size, int const& segments, int const& rings);
	void diamond(Vector3 pos, float const& vel, Vector3 const& accel, const double& damp, const float& time, float const& size, int const& segments, int const& rings);
	void sphere(Vector3 pos, float const& vel, Vector3 const& accel, const double& damp, const float& time, float const& size, int const& segments, int const& rings);


	std::vector<Particle*>* particles;
	ParticleGravity* gravity, * upward;
	ParticleForceRegistry* forceRegistry;
	Vector3 payload_pos;

	Particle* payload = nullptr;
	Fireworks fw_type = EXPLOSION_;
	bool isPayload = false;

	float fw_payload_time = 4.f;
	float fw_timer = fw_payload_time, timer_ = 0;
};