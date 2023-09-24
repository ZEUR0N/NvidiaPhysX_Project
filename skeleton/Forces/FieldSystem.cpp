#include "../Forces/FieldSystem.h"
#include <iostream>

FieldSystem::FieldSystem(std::vector<Particle*>* particles, BodySystem* bodySys, ParticleForceRegistry* fRegistry) : fRegistry_(fRegistry)
{
	particles_ = particles;
	bodySys_ = bodySys;
}

FieldSystem::~FieldSystem()
{
	for (ForceField* f : fields_)
		delete f;
}

void FieldSystem::createSphereForceField(float const& radius, float const& distance, Field fieldType, Vector3 origin)
{
	ForceField* field = new ForceField(radius, distance, fieldType, origin);
	fields_.push_back(field);
}

void FieldSystem::createBoxForceField(Vector3 const& dimensions, float const& distance, Field fieldType, Vector3 origin)
{
	ForceField* field = new ForceField(dimensions, distance, fieldType, origin);
	fields_.push_back(field);
}

void FieldSystem::update(double t)
{
	for (int i = 0; i < fields_.size(); ++i) {
		if (fields_[i]->dead()) {
			fRegistry_->clearForce(fields_[i]);

			delete fields_[i];
			fields_.erase(fields_.begin() + i);
			--i;
		}
		else {
			for (int j = 0; j < particles_->size(); j++)
			{
				if (fields_[i]->contains(particles_->at(j)) && !fields_[i]->inInfluenced(particles_->at(j)))
				{
					fields_[i]->influenced_.push_back({ particles_->at(j), false });
				}
			}
			for (int k = 0; k < fields_[i]->influenced_.size(); k++)
			{
				if (!fields_[i]->getApplied(k))
				{
					fRegistry_->add(fields_[i]->influenced_[k].p, fields_[i]);
					fields_[i]->setApplied(k);
				}

				if (!fields_[i]->contains(fields_[i]->influenced_[k].p))
				{
					fRegistry_->remove(fields_[i]->influenced_[k].p, fields_[i]);
					fields_[i]->influenced_.erase(fields_[i]->influenced_.begin() + k);
					k--;
				}
			}/*
			for (int j = 0; j < bodySys_->bodies->size(); j++)
			{
				if (fields_[i]->contains(bodySys_->bodies->at(j)) && !fields_[i]->inInfluenced(bodySys_->bodies->at(j)))
				{
					fields_[i]->influenced_RB_.push_back({ bodies->bodies->at(j), false });
				}
			}
			for (int k = 0; k < fields_[i]->influenced_RB_.size(); k++)
			{
				if (!fields_[i]->getAppliedRB(k))
				{
					fRegistry_->addNews(fields_[i]->influenced_RB_[k].rb, fields_[i]);
					fields_[i]->setAppliedRB(k);
				}

				if (!fields_[i]->contains(fields_[i]->influenced_RB_[k].rb))
				{
					fRegistry_->removeNews(fields_[i]->influenced_RB_[k].rb, fields_[i]);
					fields_[i]->influenced_RB_.erase(fields_[i]->influenced_RB_.begin() + k);
					k--;
				}
			}*/
			fields_[i]->update(t);
		}
	}
}
