#include "stdafx.h"
#include "PhysicsSimulator.h"
//-----------------------------------------------------------------------------
PhysicsSimulator gPhysicsSimulator;
//-----------------------------------------------------------------------------
PhysicsSimulator::~PhysicsSimulator()
{
	Destroy();
}
//-----------------------------------------------------------------------------
bool PhysicsSimulator::Create(const PhysicsCreateInfo& createInfo)
{
	m_gravity = createInfo.gravity;

	// broad-фаза.
	// - btDbvtBroadphase используется в динамическом мире с множеством двигающихся объектов
	// - bt32BitAxisSweep3 вариант, возможно эффективней под мой проект - для более статичных кубов. У меня динамика не планируется. при этом размер региона известен (физ движок работает в текущем регионе)
	if (createInfo.broadPhaseType == BroadPhaseType::Dbvt)
		m_broadPhase = new btDbvtBroadphase();
	else
		m_broadPhase = new bt32BitAxisSweep3(
			{ createInfo.aabbInAxisSweep.min.x, createInfo.aabbInAxisSweep.min.y, createInfo.aabbInAxisSweep.min.z },
			{ createInfo.aabbInAxisSweep.max.x, createInfo.aabbInAxisSweep.max.y, createInfo.aabbInAxisSweep.max.z });


	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadPhase, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(m_gravity.x, m_gravity.y, m_gravity.z));

	return true;
}
//-----------------------------------------------------------------------------
void PhysicsSimulator::Destroy()
{
	ClearScene(true, true, true);
	m_materials.clear();
	delete m_dynamicsWorld; m_dynamicsWorld = nullptr;
	delete m_solver; m_solver = nullptr;
	delete m_dispatcher; m_dispatcher = nullptr;
	delete m_collisionConfiguration; m_collisionConfiguration = nullptr;
	delete m_broadPhase; m_broadPhase = nullptr;
}
//-----------------------------------------------------------------------------
void PhysicsSimulator::Update(float deltaTime)
{
	m_dynamicsWorld->stepSimulation(1.f / 60.0f, 3/*10*/); // FIX FPS
}
//-----------------------------------------------------------------------------
void PhysicsSimulator::SetGravity(const glm::vec3& gravity)
{
	m_gravity = gravity;
	m_dynamicsWorld->setGravity(btVector3(m_gravity.x, m_gravity.y, m_gravity.z));
	for (auto& rb : m_rigidBodies)
		rb->SetGravity(m_gravity); // TODO: нужно ли?
}
//-----------------------------------------------------------------------------
void PhysicsSimulator::SetContactCallback(const PhysicsContactCallback& callback)
{
	m_contactCallback = callback;
}
//-----------------------------------------------------------------------------
std::shared_ptr<PhysicsMaterial> PhysicsSimulator::CreateMaterial(float StaticFriction, float DynamicFriction, float Restitution)
{
	return nullptr; // TODO:
}
//-----------------------------------------------------------------------------
void PhysicsSimulator::DeleteMaterial(std::shared_ptr<PhysicsMaterial> material)
{
	RemoveElement(m_materials, material);
}
//-----------------------------------------------------------------------------
std::shared_ptr<StaticPhysicsObject> PhysicsSimulator::CreateStaticObject(const PlaneDesc& planeDesc)
{
	std::shared_ptr<StaticPhysicsObject> ref(new StaticPhysicsObject());
	ref->CreatePlane(planeDesc.planeNormal, planeDesc.planeConstant);
	m_staticBodies.emplace_back(ref);
	return ref;
}
//-----------------------------------------------------------------------------
void PhysicsSimulator::DeleteStaticObject(std::shared_ptr<StaticPhysicsObject> object)
{
	RemoveElement(m_staticBodies, object);
}
//-----------------------------------------------------------------------------
std::shared_ptr<RigidBody> PhysicsSimulator::CreateRigidBody()
{
	return std::shared_ptr<RigidBody>();
}
//-----------------------------------------------------------------------------
void PhysicsSimulator::DeleteRigidBody(std::shared_ptr<RigidBody> object)
{
	RemoveElement(m_rigidBodies, object);
}
//-----------------------------------------------------------------------------
std::shared_ptr<PhysicsJoint> PhysicsSimulator::CreateJoint()
{
	return std::shared_ptr<PhysicsJoint>();
}
//-----------------------------------------------------------------------------
void PhysicsSimulator::DeleteJoint(std::shared_ptr<PhysicsJoint> object)
{
	RemoveElement(m_joints, object);
}
//-----------------------------------------------------------------------------
void PhysicsSimulator::ClearScene(bool rigidBodies, bool staticObjects, bool joints)
{
	if (rigidBodies) m_rigidBodies.clear();
	if (staticObjects) m_staticBodies.clear();
	if (joints) m_joints.clear();;
}
//-----------------------------------------------------------------------------
PhysicsSimulator& GetPhysicsSimulator()
{
	return gPhysicsSimulator;
}
//-----------------------------------------------------------------------------