#include "ParticleForceRegistry.h"


void ParticleForceRegistry::updateForces(float t)
{
	for (auto it = registrations.begin(); it != registrations.end(); ++it)
	{
		it->fg->updateForce(it->particle, t);
	}
	for (auto it = registrations_rigid.begin(); it != registrations_rigid.end(); ++it)
	{
		it->bfg->updateForce(it->body, t);
	}
}

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
{
	registrations.push_back({ particle, fg });
}

void ParticleForceRegistry::addNews(BodySystem* bodySys, std::vector<BodyForceGenerator*> fg)
{
	for (BodyForceGenerator* bfg : fg) {
		for (SolidBody* body : bodySys->bodies) {
				registrations_rigid.push_back({ body, bfg });
		}
	}
}


void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
{
	auto it = registrations.begin();
	bool end = false;
	while (it != registrations.end() && !end)
	{
		if ((*it).particle == particle && (*it).fg == fg) {
			it = registrations.erase(it);
			end = true;
		}
		else
			++it;
	}
}

void ParticleForceRegistry::removeNews(SolidBody* body, BodyForceGenerator* bfg)
{
	auto it = registrations_rigid.begin();
	bool end = false;
	while (it != registrations_rigid.end() && !end)
	{
		if ((*it).body == body && (*it).bfg == bfg) {
			it = registrations_rigid.erase(it);
			end = true;
		}
		else
			++it;
	}
}

void ParticleForceRegistry::clearParticle(Particle* particle)
{
	for (auto it = registrations.begin(); it != registrations.end();) {
		if ((*it).particle == particle) {
			it = registrations.erase(it);
		}
		else
			++it;
	}
}

void ParticleForceRegistry::clearBody(BodySystem* bodySys)
{
	for (auto it = registrations_rigid.begin(); it != registrations_rigid.end();) {
		int i = 0;
		while (i < bodySys->bodies.size() && (*it).body != bodySys->bodies.at(i)) ++i;
		if (i >= bodySys->bodies.size())
			it = registrations_rigid.erase(it);
		else
			++it;
	}
}

void ParticleForceRegistry::clearForce(ParticleForceGenerator* fg)
{
	for (auto it = registrations.begin(); it != registrations.end();) {
		if ((*it).fg == fg) {
			it = registrations.erase(it);
		}
		else
			++it;
	}
}

void ParticleForceRegistry::clearBodyForce(BodyForceGenerator* bfg)
{
	for (auto it = registrations_rigid.begin(); it != registrations_rigid.end();) {
		if ((*it).bfg == bfg) {
			it = registrations_rigid.erase(it);
		}
		else
			++it;
	}
}

void ParticleForceRegistry::clear()
{
	registrations.clear();
	registrations_rigid.clear();
}
