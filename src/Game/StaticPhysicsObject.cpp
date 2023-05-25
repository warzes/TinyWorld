#include "stdafx.h"
#include "StaticPhysicsObject.h"
#include "PhysicsSimulator.h"
#include "BulletUtility.h"
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

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0, 0, 0));

	btScalar mass(0.f);
	btVector3 localInertia(0, 0, 0);
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
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
btTransform StaticPhysicsObject::GetBTTransform() const
{
	assert(m_body);
	btTransform trans;
	if (m_body->getMotionState())
		m_body->getMotionState()->getWorldTransform(trans);
	else
		trans = m_body->getWorldTransform();

	return trans;
}
//-----------------------------------------------------------------------------
glm::mat4 StaticPhysicsObject::GetMatrix() const
{
	// чтобы не создавать кучу временных объектов надо просто написать свой MotionState вместо юзанья btDefaultMotionState
	// также возможно в getOpenGLMatrix можно передать саму glm::mat4 скастовав в флоаты
	static btScalar mat[16];
	GetBTTransform().getOpenGLMatrix(mat);
	return btScalarTomat4(mat);
}
//-----------------------------------------------------------------------------