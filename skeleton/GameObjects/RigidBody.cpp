#include "RigidBody.h"



RigidBody::RigidBody(PxPhysics* _gPhysics, PxScene* _gScene, PxTransform tr_, Vector4 color_, bool dynamic, float lifetime_, float mass_, float lDamp_, float aDamp_):
    tr(tr_), color(color_), lifetime(lifetime_), mass(mass_), linearDamping(lDamp_), angularDamping(aDamp_)
{
    mat = _gPhysics->createMaterial(0.f, 0.f, 1.f);
    rb = _gPhysics->createRigidDynamic(tr);
    rb->createShape(PxSphereGeometry(2), *mat);
    PxShape* shape = CreateShape(PxSphereGeometry(2));
    rb->setMass(mass);
    //rb->attachShape(*shape);
    _gScene->addActor(*rb);
    renderItem = new RenderItem(shape, rb, color);

    inverseMass = 1 / mass;
}

RigidBody::~RigidBody()
{
    DeregisterRenderItem(renderItem);
    delete renderItem;
}


void RigidBody::setInertiaTensor(const PxMat33& inertiaTensor)
{
    inverseInertiaTensor = inertiaTensor.getInverse();
}


PxMat33 RigidBody::getWorldInverseInertiaTensor() const
{
    PxMat33 R(orientation);
    PxMat33 invR = R.getTranspose();
    PxMat33 Iw = R * inverseInertiaTensor;
    Iw = Iw * invR;
    return Iw;

}

void RigidBody::addForce(const Vector3& _f)
{
    rb->addForce(_f);
}

void RigidBody::addForceAtPoint(const Vector3& _f, const Vector3& _p)
{
    Vector3 pt = _p;
    pt -= tr.p;
    force += _f;
    torque += pt.cross(_f);
}

void RigidBody::addForceAtLocalPoint(const Vector3& _f, const Vector3& _p) {
    Vector3 pt = tr.transform(_p);
    addForceAtPoint(_f, pt);
}

void RigidBody::clearAccumulators() {

    rb->clearForce();
}

Vector3 RigidBody::getPosition()
{
    return rb->getGlobalPose().p;
}

void RigidBody::setPosition(Vector3 p)
{
    rb->setGlobalPose(PxTransform(p));
}

void RigidBody::setColor(Vector4 c)
{
    renderItem->color = c;
}
