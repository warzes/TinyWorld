#include "stdafx.h"
#include "RigidBody.h"
#include "PhysicsSimulator.h"
#include "BulletUtility.h"
//-----------------------------------------------------------------------------
RigidBody::~RigidBody()
{
	Destroy();
}
//-----------------------------------------------------------------------------
void RigidBody::CreateBox(const glm::vec3& boxHalfExtents, float mass, const glm::vec3& pos, const glm::quat& rot)
{
	assert(GetPhysicsSimulator().GetWorld());

	m_mass = mass;

	m_shape = new btBoxShape({ boxHalfExtents.x, boxHalfExtents.y, boxHalfExtents.z });

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin({ pos.x, pos.y, pos.z });
	transform.setRotation({ rot.x, rot.y, rot.z, rot.w });

	// rigidbody is dynamic if and only if mass is non zero, otherwise static
	const bool isDynamic = (m_mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic) m_shape->calculateLocalInertia(m_mass, localInertia);
	m_localInertia = { localInertia.x(), localInertia.y(), localInertia.z()};

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, motionState, m_shape, localInertia);

	m_body = new btRigidBody(rbInfo);

	GetPhysicsSimulator().GetWorld()->addRigidBody(m_body);
}
//-----------------------------------------------------------------------------
void RigidBody::Destroy()
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
void RigidBody::SetGravity(const glm::vec3& gravity)
{
	if (m_body) m_body->setGravity({ gravity.x, gravity.y, gravity.z });
}
//-----------------------------------------------------------------------------
void RigidBody::SetMass(float mass)
{
	btVector3 LocalInertia(0, 0, 0);
	m_shape->calculateLocalInertia(mass, LocalInertia);
	m_localInertia = { LocalInertia.x(), LocalInertia.y(), LocalInertia.z() };
	m_body->setMassProps(mass, LocalInertia);
}
//-----------------------------------------------------------------------------
void RigidBody::SetAutoSleep(bool sleep)
{
	if (m_body) m_body->setActivationState(sleep ? WANTS_DEACTIVATION : DISABLE_DEACTIVATION);
}
//-----------------------------------------------------------------------------
void RigidBody::SetMassCenter(const glm::vec3& localPoint)
{
	if (m_body) m_body->setMassProps(m_mass, { localPoint.x, localPoint.y, localPoint.z });
	m_localInertia = localPoint;
}
//-----------------------------------------------------------------------------
void RigidBody::AddVelocity(const glm::vec3& direction)
{
	if (m_body) m_body->applyCentralImpulse({ direction.x * m_mass, direction.y * m_mass, direction.z * m_mass });
}
//-----------------------------------------------------------------------------
void RigidBody::AddImpulse(const glm::vec3& direction, const glm::vec3& pivotPoint)
{
	if (m_body)
	{
		m_body->applyImpulse(
			{ direction.x * m_mass, direction.y * m_mass, direction.z * m_mass },
			{ pivotPoint.x, pivotPoint.y, pivotPoint.z }
		);
	}
}
//-----------------------------------------------------------------------------
btTransform RigidBody::GetBTTransform() const
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
glm::mat4 RigidBody::GetMatrix() const
{
	// чтобы не создавать кучу временных объектов надо просто написать свой MotionState вместо юзанья btDefaultMotionState
	// также возможно в getOpenGLMatrix можно передать саму glm::mat4 скастовав в флоаты
	static btScalar mat[16];
	GetBTTransform().getOpenGLMatrix(mat);
	return btScalarTomat4(mat);
}
//-----------------------------------------------------------------------------