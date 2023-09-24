#pragma once
#include "../utils/core.hpp"
#include "../utils/RenderUtils.hpp"
#include "../GameObjects/Particle.h"
#include "../GameObjects/BodySystem.h"
#include "../utils/Color.h"

#include "ParticleForceGenerator.h"
#include "ParticleForces.h"
#include "BodyForces.h"


enum Field {
	WIND, GRAVITYWELL, EXPLOSION, WHIRLWIND, BOUYANCE
};
struct Pair
{
	Particle* p;
	bool forceApplied;
};
struct PairRB
{
	SolidBody* rb;
	bool forceApplied;
};
class ForceField : public ParticleForceGenerator
{
	Vector3 f_;
public:
	ForceField(float const& radius, float const& distance, Field fieldType, Vector3 origin = Vector3(0));
	ForceField(Vector3 const& dimensions, float const& distance, Field fieldType, Vector3 origin = Vector3(0));
	~ForceField();

	void setColor(rgb col) { renderItem_->color = { col.r, col.g, col.b, 0.0f }; };
	Vector4 getColor() { return renderItem_->color; };

	bool getApplied(int i) const { return influenced_[i].forceApplied; };
	void setApplied(int i) { influenced_[i].forceApplied = true; };

	bool getAppliedRB(int i) const { return influenced_RB_[i].forceApplied; };
	void setAppliedRB(int i) { influenced_RB_[i].forceApplied = true; };

	bool contains(Particle* p) const;
	bool contains(SolidBody* rb) const;
	bool dead()const { return dead_; };

	void update(const double& t);
	virtual void updateForce(Particle* p, float t);
	virtual void updateForce(SolidBody* rb, float t);

	bool inInfluenced(Particle* p) {
		bool found = false;
		int i = 0;
		while (i < influenced_.size() && !found)
		{
			if (influenced_[i].p == p)
			{
				found = true;
			}
			i++;
		}
		return found;
	}

	bool inInfluenced(SolidBody* rb) {
		bool found = false;
		int i = 0;
		while (i < influenced_RB_.size() && !found)
		{
			if (influenced_RB_[i].rb == rb)
			{
				found = true;
			}
			i++;
		}
		return found;
	}

	std::vector<Pair> influenced_;
	std::vector<PairRB> influenced_RB_;
private:
	Vector3 origin_;
	float r_;
	Vector3 d_;
	ParticleForceGenerator* fg;

	RenderItem* renderItem_;
	physx::PxTransform pose_;
	Field fType_;

	float time_;
	float counter_;
	bool dead_ = false;
};