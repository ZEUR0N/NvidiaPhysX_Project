#include"../Forces/ForceField.h"

ForceField::ForceField(float const& radius, float const& distance, Field fieldType, Vector3 origin) :
	r_(radius), fType_(fieldType), origin_(origin)
{
	counter_ = 0;
	time_ = 10.0f;

	Vector3 color;

	if (origin.x == 0 && origin.y == 0 && origin.z == 0) {
		origin_ = {
			GetCamera()->getTransform().p.x + GetCamera()->getDir().x * distance,
			GetCamera()->getTransform().p.y + GetCamera()->getDir().y * distance,
			GetCamera()->getTransform().p.z + GetCamera()->getDir().z * distance };
	}

	switch (fieldType)
	{
	case WIND: {
		color = { 0, 0.8f, 0.4f };
		f_ = { 0,-10,0 };
		break;
	}
	case GRAVITYWELL: {
		color = { 0.8, 0.2f, 0.8f };
		f_ = { 1, 1, 1 };
		break;
	}
	case EXPLOSION: {
		color = { 1.0f, 0.2f, 0 };
		f_ = { 1, 1, 1 };
		time_ = 0.1f;
		break;
	}
	case WHIRLWIND: {
		color = { 1.0f, 1.0f, 1.0f };
		f_ = { 1, 1, 1 };
		time_ = 1.0f;
		break;
	}
	case BOUYANCE: {
		color = { 0.0f, 0.5f, 1.0f };
		fg = new ParticleBuoyancy(radius, 10, radius , 4, origin_.y);
		time_ = 15.0f;
		break;
	}
	default:
		break;
	}

	pose_ = physx::PxTransform(origin_.x, origin_.y, origin_.z);
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(r_));

	renderItem_ = new RenderItem(shape, &pose_, { color.x,color.y,color.z,0.0f });
}

ForceField::ForceField(Vector3 const& dimensions, float const& distance, Field fieldType, Vector3 origin) :
	d_(dimensions), fType_(fieldType), origin_(origin)
{
	counter_ = 0;
	time_ = 10.0f;

	Vector3 color;
	if (origin.x == 0 && origin.y == 0 && origin.z == 0) {
		origin_ = {
			GetCamera()->getTransform().p.x + GetCamera()->getDir().x * distance,
			GetCamera()->getTransform().p.y + GetCamera()->getDir().y * distance,
			GetCamera()->getTransform().p.z + GetCamera()->getDir().z * distance };
	}

	switch (fieldType)
	{
	case WIND: {
		color = { 0, 0.8f, 0.4f };
		f_ = { 0,-10,0 };
		break;
	}
	case GRAVITYWELL: {
		color = { 0.8, 0.2f, 0.8f };
		f_ = { 1, 1, 1 };
		break;
	}
	case EXPLOSION: {
		color = { 1.0f, 0.2f, 0 };
		f_ = { 1, 1, 1 };
		time_ = 0.1f;
		break;
	}
	case WHIRLWIND: {
		color = { 1.0f, 1.0f, 1.0f };
		f_ = { 1, 1, 1 };
		time_ = 1.0f;
		break;
	}
	case BOUYANCE: {
		color = { 0.0f, 0.5f, 1.0f };
		fg = new ParticleBuoyancy(d_.y, 10, 2, 3, origin_.y);
		time_ = 20.0f;
		break;
	}
	default:
		break;
	}

	pose_ = physx::PxTransform(origin_.x, origin_.y, origin_.z);
	physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(dimensions));

	
	renderItem_ = new RenderItem(shape, &pose_, { color.x,color.y,color.z, 1.0f });
}

ForceField::~ForceField()
{
	DeregisterRenderItem(renderItem_);
	delete renderItem_;
}

bool ForceField::contains(Particle* p) const
{
	if(r_ > 0)
		return ((p->getPosition() - origin_).magnitude() < r_);
	else {
		return(p->getPosition().x >= origin_.x - d_.x && p->getPosition().x <= origin_.x + d_.x &&
			p->getPosition().y >= origin_.y - d_.y && p->getPosition().y <= origin_.y + d_.y &&
			p->getPosition().z >= origin_.z - d_.z && p->getPosition().z <= origin_.z + d_.z);
	}
}

bool ForceField::contains(SolidBody* rb) const
{
	if (r_ > 0)
		return ((rb->rigid->getGlobalPose().p - origin_).magnitude() < r_);
	else {
		return(rb->rigid->getGlobalPose().p.x >= origin_.x - d_.x && rb->rigid->getGlobalPose().p.x <= origin_.x + d_.x &&
			rb->rigid->getGlobalPose().p.y >= origin_.y - d_.y && rb->rigid->getGlobalPose().p.y <= origin_.y + d_.y &&
			rb->rigid->getGlobalPose().p.z >= origin_.z - d_.z && rb->rigid->getGlobalPose().p.z <= origin_.z + d_.z);
	}
}

void ForceField::update(const double& t)
{
	if (counter_ > time_) {
		dead_ = true;
	}
	else {
		counter_ += t;
	}
}

void ForceField::updateForce(Particle* p, float t)
{
	if (!p->hasFiniteMass()) return;
	if (fType_ == WIND)
		p->addForce(f_);
	if (fType_ == GRAVITYWELL)
	{
		f_ = {
		(p->getPosition().x - origin_.x) * -50,
		(p->getPosition().y - origin_.y) * -50,
		(p->getPosition().z - origin_.z) * -50,
		};
		p->addForce(f_);
	}
	if (fType_ == EXPLOSION)
	{
		physx::PxVec3 v = (p->getPosition() - origin_);
		f_ = v.getNormalized() * (r_ - v.magnitude()) * 100.0f;
		p->addForce(f_);
	}
	if (fType_ == WHIRLWIND)
	{
		physx::PxVec3 v = p->getPosition() - origin_;
		physx::PxVec3 vVertical = (Vector3(origin_.x, origin_.y + 20, origin_.z) - origin_);

		Vector3 crossPr = {
			v.cross(vVertical).x,
			v.cross(vVertical).y,
			v.cross(vVertical).z
		};
		Vector3 centripetal = {
		(p->getPosition().x - origin_.x) * -50,
		0.0f,
		(p->getPosition().z - origin_.z) * -50
		};

		f_ = crossPr + centripetal;
		p->addForce(f_);
	}
	if (fType_ == BOUYANCE) {
		fg->updateForce(p, t);
	}
}

void ForceField::updateForce(SolidBody* rb, float t)
{
	if (fType_ == WIND)
		rb->force += f_;
	if (fType_ == GRAVITYWELL)
	{
		f_ = {
		(rb->rigid->getGlobalPose().p.x - origin_.x) * -50,
		(rb->rigid->getGlobalPose().p.y - origin_.y) * -50,
		(rb->rigid->getGlobalPose().p.z - origin_.z) * -50,
		};
		rb->force += f_;
	}
	if (fType_ == EXPLOSION)
	{
		physx::PxVec3 v = (rb->rigid->getGlobalPose().p - origin_);
		f_ = v.getNormalized() * (r_ - v.magnitude()) * 100.0f;
		rb->force += f_;
	}
	if (fType_ == WHIRLWIND)
	{
		physx::PxVec3 v = rb->rigid->getGlobalPose().p - origin_;
		physx::PxVec3 vVertical = (Vector3(origin_.x, origin_.y + 20, origin_.z) - origin_);

		Vector3 crossPr = {
			v.cross(vVertical).x,
			v.cross(vVertical).y,
			v.cross(vVertical).z
		};
		Vector3 centripetal = {
		(rb->rigid->getGlobalPose().p.x - origin_.x) * -50,
		0.0f,
		(rb->rigid->getGlobalPose().p.z - origin_.z) * -50
		};

		f_ = crossPr + centripetal;
		rb->force += f_;
	}
}
