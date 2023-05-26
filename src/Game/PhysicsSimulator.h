#pragma once

#include "RigidBody.h"
#include "StaticPhysicsObject.h"
#include "PhysicsMaterial.h"
#include "PhysicsJoint.h"
#include "CharacterController.h"
#include "ShapeDesc.h"

typedef std::function<void(const glm::vec3& point, const glm::vec3& normal, float Velocity)> PhysicsContactCallback;

enum class BroadPhaseType
{
	Dbvt,
	AxisSweep3
};

struct PhysicsCreateInfo
{
	BroadPhaseType broadPhaseType = BroadPhaseType::Dbvt;
	AABB worldSize = { glm::vec3(-1000.0f), glm::vec3(1000.0f) }; // только для AxisSweep3

	glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
};

struct PhysicsRayInfo
{

};

class PhysicsSimulator
{
public:
	~PhysicsSimulator();

	bool Create(const PhysicsCreateInfo& createInfo);
	void Destroy();

	void Update(float deltaTime);

	void SetGravity(const glm::vec3& gravity);
	void SetContactCallback(const PhysicsContactCallback& callback);

	PhysicsMaterialRef CreateMaterial(float StaticFriction = 0.5f, float DynamicFriction = 0.5f, float Restitution = 0.3f);
	void Delete(PhysicsMaterialRef material);

	StaticPhysicsObjectRef CreateStaticObject(const PlaneDesc& planeDesc);
	StaticPhysicsObjectRef CreateStaticObject(const ConvexDesc& convexDesc, const glm::vec3& pos = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	StaticPhysicsObjectRef CreateStaticObject(const TriangleMeshDesc& meshDesc, const glm::vec3& pos = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	void Delete(StaticPhysicsObjectRef object);

	RigidBodyRef CreateRigidBody(const BoxDesc& boxDesc, float mass = 1.0f, const glm::vec3& pos = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	RigidBodyRef CreateRigidBody(const SphereDesc& sphereDesc, float mass = 1.0f, const glm::vec3& pos = glm::vec3(0.0f));
	RigidBodyRef CreateRigidBody(const CapsuleDesc& capsuleDesc, float mass = 1.0f, const glm::vec3& pos = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	RigidBodyRef CreateRigidBody(const ConeDesc& coneDesc, float mass = 1.0f, const glm::vec3& pos = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	RigidBodyRef CreateRigidBody(const ConvexDesc& convexDesc, float mass = 1.0f, const glm::vec3& pos = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	RigidBodyRef CreateRigidBody(const TriangleMeshDesc& meshDesc, float mass = 1.0f, const glm::vec3& pos = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	void Delete(RigidBodyRef object);

	PhysicsJointRef CreateJoint();
	void Delete(PhysicsJointRef object);

	CharacterControllerRef CreateCharacterController();
	void Delete(CharacterControllerRef character);

	bool CastRay(const glm::vec3& startPnt, const glm::vec3& endPnt, PhysicsRayInfo* ri, const glm::vec3& impulse);

	void ClearScene(bool rigidBodies = true, bool staticObjects = true, bool joints = true, bool character = true);

	glm::vec3 GetGravity() const { return m_gravity; }
	std::list<RigidBodyRef> GetRigidBodyList() const { return m_rigidBodies; }
	std::list<StaticPhysicsObjectRef> GetStaticBodyList() const { return m_staticBodies; }
	std::list<PhysicsMaterialRef> GetMaterialList() const { return m_materials; }
	std::list<PhysicsJointRef> GetJointList() const { return m_joints; }

	btDiscreteDynamicsWorld* GetWorld() { return m_dynamicsWorld; }
private:
	glm::vec3 m_gravity;
	PhysicsContactCallback m_contactCallback = nullptr;

	btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
	btCollisionDispatcher* m_dispatcher = nullptr;
	btBroadphaseInterface* m_broadPhase = nullptr;
	btSequentialImpulseConstraintSolver* m_solver = nullptr;
	btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;

	std::list<RigidBodyRef> m_rigidBodies;
	std::list<StaticPhysicsObjectRef> m_staticBodies;
	std::list<PhysicsMaterialRef> m_materials;
	std::list<PhysicsJointRef> m_joints;
	std::list<CharacterControllerRef> m_characters;
};

PhysicsSimulator& GetPhysicsSimulator();