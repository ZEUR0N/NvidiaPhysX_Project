#include "Particle.h"

Particle::Particle(const Vector3& pos, const Vector3& vel, const Vector3& accel, const double& damp, const float& time, float const& size, Vector4 const& color, float inversemass, bool isPayload)
	: v_(vel), a_(accel), damp_(damp), time_(time), size_(size), color_(color), inverseMass_(inversemass), isPayload_(isPayload)
{
	pose = physx::PxTransform(pos.x, pos.y, pos.z);

	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(size));
	renderItem_ = new RenderItem(shape, &pose, color_);

	counter_ = 0;
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem_);
	delete renderItem_;
}

void Particle::update(const double& t)
{
	if (counter_ > time_) {
		dead_ = true;
	}
	else
		counter_ += t;
							  // if infinite mass do nothing
	if (inverseMass_ <= 0.0001f) return;

	// Update position
	pose.p += v_ * t;

	Vector3 totalAcceleration = a_;
	a_ = force * inverseMass_;

	// Update linear velocity and impose drag
	v_ += totalAcceleration * t;
	v_ *= powf(damp_, t);

	clearForce();
}