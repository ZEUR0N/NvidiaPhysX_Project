#pragma once
#include "../utils/core.hpp"
#include "../utils/RenderUtils.hpp"
#include <vector>

class Particle
{
public:
	Particle(const Vector3& pos, const Vector3& vel, const Vector3& accel, const double& damp, const float& time, float const& size, Vector4 const& color, float inversemass, bool isPayload = false);
	~Particle();

	void update(const double& t);
	//void integrate(float t);

	bool dead() const { return dead_; };

	bool hasFiniteMass() const { return (inverseMass_ > 0.00f); };
	double getMass() const { return 1 / inverseMass_; };
	Vector3 getVelocity() const { return v_; };

	void setShape(physx::PxShape* sh) { if (renderItem_ != nullptr) renderItem_->shape = sh; };
	void setColor(Vector4 col) { renderItem_->color = { col.x, col.y, col.z, col.w }; };
	Vector4 getColor() { return renderItem_->color; }
	// Clears accumulated force
	void clearForce() { force = Vector3(0); };
	// Add force to apply in the next integration only
	void addForce(const Vector3& f) {
		force += f;
	};

	Vector3 getPosition() const& { return pose.p; ; }
	physx::PxTransform getPos() const& { return pose; };

	bool IsPayload() { return isPayload_; };
private:
	physx::PxTransform pose;
	// Linear velocity in world space
	Vector3 v_;
	// Linear acceleration in world space
	Vector3 a_;
	// How much linear velocity is lost.
	double damp_;

	float inverseMass_;

	bool isPayload_;
	// Accumulated force
	Vector3 force = Vector3(0);

	RenderItem* renderItem_;
	Vector4 color_;

	float size_;

	float time_;
	float counter_;
	bool dead_ = false;
};