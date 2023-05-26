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
	m_shapeType = PhysicsObjectShapeType::Plane;
	m_shape = new btStaticPlaneShape({ planeNormal.x, planeNormal.y, planeNormal.z }, planeConstant);
	create(glm::vec3(0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}
//-----------------------------------------------------------------------------
void StaticPhysicsObject::CreateConvex(const std::vector<glm::vec3>& points, const glm::vec3& position, const glm::quat& rotation)
{
	m_shapeType = PhysicsObjectShapeType::Convex;
	m_shape = new btConvexHullShape((btScalar*)points.data(), points.size(), sizeof(glm::vec3));
	create(position, rotation);
}
//-----------------------------------------------------------------------------
void StaticPhysicsObject::CreateTriangleMesh(const std::vector<glm::vec3>& vert, const std::vector<unsigned>& index, const glm::vec3& position, const glm::quat& rotation)
{
	m_shapeType = PhysicsObjectShapeType::TriangleMesh;
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
	create(position, rotation);
}
//-----------------------------------------------------------------------------
void StaticPhysicsObject::Destroy()
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
		if (m_shapeType == PhysicsObjectShapeType::TriangleMesh)
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
void StaticPhysicsObject::create(const glm::vec3& pos, const glm::quat& rotation)
{
	assert(GetPhysicsSimulator().GetWorld());

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin({ pos.x, pos.y, pos.z });
	transform.setRotation({ rotation.x, rotation.y, rotation.z, rotation.w });

	const btScalar mass(0.f);
	const btVector3 localInertia(0, 0, 0);
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, m_shape, localInertia);

	m_body = new btRigidBody(rbInfo);

	GetPhysicsSimulator().GetWorld()->addRigidBody(m_body);
}
//-----------------------------------------------------------------------------