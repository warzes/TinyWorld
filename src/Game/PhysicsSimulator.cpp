﻿#include "stdafx.h"
#include "PhysicsSimulator.h"
//-----------------------------------------------------------------------------
const float PhysicsStepTime = 1.0f / 60.f; //default 60fps
const unsigned PhysicsMaxSubSteps = 4;
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
			{ createInfo.worldSize.min.x, createInfo.worldSize.min.y, createInfo.worldSize.min.z },
			{ createInfo.worldSize.max.x, createInfo.worldSize.max.y, createInfo.worldSize.max.z });

	m_broadPhase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());


	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadPhase, m_solver, m_collisionConfiguration);
	if (!m_dynamicsWorld)
	{
		LogError("BtWorld failed to create dynamics world!");
		return false;
	}

	m_dynamicsWorld->setGravity(btVector3(m_gravity.x, m_gravity.y, m_gravity.z));

	// Removing the randomization in the solver is required to make the simulation deterministic.
	m_dynamicsWorld->getSolverInfo().m_solverMode &= ~SOLVER_RANDMIZE_ORDER;

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
	//const float elapsedSec = deltaTime * 0.001f;

	puts(std::to_string(deltaTime).c_str());

	m_dynamicsWorld->stepSimulation(deltaTime, PhysicsMaxSubSteps, PhysicsStepTime);
	//m_dynamicsWorld->stepSimulation(deltaTime, 10); // FIX FPS
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
std::shared_ptr<RigidBody> PhysicsSimulator::CreateRigidBody(const BoxDesc& boxDesc, float mass, const glm::vec3& pos, const glm::quat& rotation)
{
	std::shared_ptr<RigidBody> ref(new RigidBody());
	ref->CreateBox(boxDesc.boxHalfExtents, mass, pos, rotation);
	m_rigidBodies.emplace_back(ref);
	return ref;
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
bool PhysicsSimulator::CastRay(const glm::vec3& startPnt, const glm::vec3& endPnt, PhysicsRayInfo* ri, const glm::vec3& impulse)
{
	btCollisionWorld::ClosestRayResultCallback result({ startPnt.x, startPnt.y, startPnt.z }, { endPnt.x, endPnt.y, endPnt.z });
	m_dynamicsWorld->rayTest({ startPnt.x, startPnt.y, startPnt.z }, { endPnt.x, endPnt.y, endPnt.z }, result);
	if (!result.hasHit() || !result.m_collisionObject)
		return false;

	//if (ri)
	//{
	//	ri->object = PhysicsUserData::getObject(result.m_collisionObject->getUserPointer());
	//	if (ri->object == NULL)
	//		return false;

	//	ri->distance = (endPnt - startPnt).len() * result.m_closestHitFraction;
	//	ri->normal = btCast<Point3F>(result.m_hitNormalWorld);
	//	ri->point = btCast<Point3F>(result.m_hitPointWorld);
	//	ri->t = result.m_closestHitFraction;
	//}

	return true;
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