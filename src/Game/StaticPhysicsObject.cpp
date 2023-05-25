#include "stdafx.h"
#include "StaticPhysicsObject.h"
#include "PhysicsSimulator.h"
//-----------------------------------------------------------------------------
StaticPhysicsObject::~StaticPhysicsObject()
{
	Destroy();
}
//-----------------------------------------------------------------------------
void StaticPhysicsObject::CreatePlane(const glm::vec3& planeNormal, float planeConstant)
{
	assert(GetPhysicsSimulator().GetWorld());

	m_shape = new btStaticPlaneShape({ planeNormal.x, planeNormal.y,planeNormal.z }, planeConstant);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, 0, 0));

	btScalar mass(0.f);
	btVector3 localInertia(0, 0, 0);
	btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, m_shape, localInertia);

	m_body = new btRigidBody(rbInfo);

	GetPhysicsSimulator().GetWorld()->addRigidBody(m_body);
}
//-----------------------------------------------------------------------------
void StaticPhysicsObject::Destroy()
{
	assert(GetPhysicsSimulator().GetWorld());

	if (m_body && m_body->getMotionState())
		delete m_body->getMotionState();

	GetPhysicsSimulator().GetWorld()->removeCollisionObject(m_body);
	delete m_body;
	m_body = nullptr;

	delete m_shape;
	m_shape = nullptr;
}
//-----------------------------------------------------------------------------