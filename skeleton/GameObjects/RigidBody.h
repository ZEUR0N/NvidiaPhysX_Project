#pragma once

#include "../utils/core.hpp"
#include "../utils/RenderUtils.hpp"
#include <vector>

using namespace std;
using namespace physx;

class RigidBody
{
public:
	RigidBody(PxPhysics* _gPhysics, PxScene* _gScene, PxTransform tr_, Vector4 color_, bool dynamic = true, float lifetime_ = -1.f, float mass_ = 0.1f, float lDamp_ = 0.4, float aDamp_ = 0.05);
	~RigidBody();
	void update(float t) {};

	void setInertiaTensor(const PxMat33& inertiaTensor);
	PxMat33 getWorldInverseInertiaTensor() const;
	// Apply force in the center of mass
	void addForce(const Vector3& _f);
	// Apply force at point
	void addForceAtPoint(const Vector3& _f, const Vector3& _p);
	void addForceAtLocalPoint(const Vector3& _f, const Vector3& _p);
	// clear forces and torques
	void clearAccumulators();

	Vector3 getPosition();
	void setPosition(Vector3 p);

	void setColor(Vector4 c);

private:
	PxRigidDynamic* rb;
	PxMaterial* mat;
	// Inverse tensor in local coordinates
	PxMat33 inverseInertiaTensor  ;
	PxQuat orientation = PxQuat(0.5);
	Vector3 force = Vector3(0);
	Vector3 torque = Vector3(0);

	PxTransform tr;
	// Linear velocity in world space
	Vector3 v = Vector3(0);
	// How much linear velocity is lost.
	double linearDamping;
	float angularDamping;

	float mass;
	float inverseMass;

	RenderItem* renderItem;
	Vector4 color;

	float lifetime;
	bool dead = false;

};