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
void RigidBody::CreateBox(const glm::vec3& boxHalfExtents, float mass, const glm::vec3& position, const glm::quat& rotation)
{
	m_shapeType = RigidBodyShapeType::Box;
	m_shape = new btBoxShape({ boxHalfExtents.x, boxHalfExtents.y, boxHalfExtents.z });
	create(mass, position, rotation);
}
//-----------------------------------------------------------------------------
void RigidBody::CreateSphere(float radius, float mass, const glm::vec3& position)
{
	m_shapeType = RigidBodyShapeType::Sphere;
	m_shape = new btSphereShape(radius);
	create(mass, position, glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}
//-----------------------------------------------------------------------------
void RigidBody::CreateCapsule(float radius, float height, float mass, const glm::vec3& position, const glm::quat& rotation)
{
	m_shapeType = RigidBodyShapeType::Capsule;
	m_shape = new btCapsuleShape(radius, height);
	create(mass, position, rotation);
}
//-----------------------------------------------------------------------------
void RigidBody::CreateCone(float radius, float height, float mass, const glm::vec3& position, const glm::quat& rotation)
{
	m_shapeType = RigidBodyShapeType::Cone;
	m_shape = new btConeShape(radius, height);
	create(mass, position, rotation);
}
//-----------------------------------------------------------------------------
void RigidBody::CreateConvex(const std::vector<glm::vec3>& points, float mass, const glm::vec3& position, const glm::quat& rotation)
{
	m_shapeType = RigidBodyShapeType::Convex;
	m_shape = new btConvexHullShape((btScalar*)points.data(), points.size(), sizeof(glm::vec3));
	create(mass, position, rotation);
}
//-----------------------------------------------------------------------------
void RigidBody::CreateTriangleMesh(const std::vector<glm::vec3>& vert, const std::vector<unsigned>& index, float mass, const glm::vec3& position, const glm::quat& rotation)
{
	m_shapeType = RigidBodyShapeType::TriangleMesh;
	const size_t triCount = index.size() / 3;

	btTriangleMesh* meshInterface = new btTriangleMesh(true, false);
	for (size_t i = 0; i < triCount; i++)
	{
		const unsigned index0 = index[i * 3 + 0];
		const unsigned index1 = index[i * 3 + 1];
		const unsigned index2 = index[i * 3 + 2];

		meshInterface->addTriangle(
			{ vert[index0].x, vert[index0].y, vert[index0].z },
			{ vert[index1].x, vert[index1].y, vert[index1].z },
			{ vert[index2].x, vert[index2].y, vert[index2].z },
			false
		);
	}

	m_shape = new btBvhTriangleMeshShape(meshInterface, true, true);
	create(mass, position, rotation);
}
//-----------------------------------------------------------------------------
void RigidBody::Destroy()
{
	assert(GetPhysicsSimulator().GetWorld());

	if (m_body)
	{
		if (m_body->getMotionState())
			delete m_body->getMotionState();

		GetPhysicsSimulator().GetWorld()->removeRigidBody(m_body);
		m_body->setUserPointer(nullptr);
		delete m_body;
		m_body = nullptr;
	}

	if (m_shape)
	{
		if (m_shapeType == RigidBodyShapeType::TriangleMesh)
		{
			auto triangleMeshShape = dynamic_cast<btBvhTriangleMeshShape*>(m_shape);
			if (triangleMeshShape)
			{
				auto triangleMesh = static_cast<btTriangleMesh*>(triangleMeshShape->getMeshInterface());
				delete triangleMesh;
			}
		}

		delete m_shape;
		m_shape = nullptr;
	}
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
void RigidBody::SetMassCenter(const glm::vec3& localPoint)
{
	if (m_body) m_body->setMassProps(m_mass, { localPoint.x, localPoint.y, localPoint.z });
	m_localInertia = localPoint;
}
//-----------------------------------------------------------------------------
void RigidBody::SetSleeping(bool sleeping)
{
	if (!m_body) return;
	if (sleeping)
	{
		m_body->setActivationState(WANTS_DEACTIVATION);
		m_body->setDeactivationTime(0.0f);
	}
	else
	{
		m_body->setActivationState(DISABLE_DEACTIVATION);
		m_body->activate();
	}
}
//-----------------------------------------------------------------------------
void RigidBody::SetSleepThreshold(float linear, float angular)
{
	if (m_body) m_body->setSleepingThresholds(linear, angular);
}
//-----------------------------------------------------------------------------
void RigidBody::SetDamping(float linear, float angular)
{
	if (m_body) m_body->setDamping(linear, angular);
}
//-----------------------------------------------------------------------------
void RigidBody::SetMaterial(float restitution, float friction)
{
	m_body->setRestitution(restitution);
	m_body->setFriction(friction);
	m_body->activate();
}
//-----------------------------------------------------------------------------
void RigidBody::SetLinearFactor(const glm::vec3& factor)
{
	m_body->setLinearFactor({ factor.x, factor.y, factor.z });
}
//-----------------------------------------------------------------------------
void RigidBody::SetAngularFactor(const glm::vec3& factor)
{
	m_body->setAngularFactor({ factor.x, factor.y, factor.z });
}
//-----------------------------------------------------------------------------
void RigidBody::SetLinearVelocity(const glm::vec3& velocity)
{
	if (m_body) m_body->setLinearVelocity({ velocity.x, velocity.y, velocity.z });
}
//-----------------------------------------------------------------------------
void RigidBody::SetAngularVelocity(const glm::vec3& velocity)
{
	if (m_body) m_body->setAngularVelocity({ velocity.x, velocity.y, velocity.z });
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
void RigidBody::create(float mass, const glm::vec3& pos, const glm::quat& rot)
{
	assert(GetPhysicsSimulator().GetWorld());

	m_mass = mass;

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin({ pos.x, pos.y, pos.z });
	transform.setRotation({ rot.x, rot.y, rot.z, rot.w });

	// rigidbody is dynamic if and only if mass is non zero, otherwise static
	const bool isDynamic = (m_mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic) m_shape->calculateLocalInertia(m_mass, localInertia);
	m_localInertia = { localInertia.x(), localInertia.y(), localInertia.z() };

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, motionState, m_shape, localInertia);

	m_body = new btRigidBody(rbInfo);

	GetPhysicsSimulator().GetWorld()->addRigidBody(m_body);
}
//-----------------------------------------------------------------------------