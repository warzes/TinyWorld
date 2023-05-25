#pragma once

#include "RigidBody.h"
#include "StaticPhysicsObject.h"
#include "PhysicsMaterial.h"
#include "PhysicsJoint.h"
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
	AABB aabbInAxisSweep = { glm::vec3(-500.0f), glm::vec3(500.0f) }; // только для AxisSweep3

	glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
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

	std::shared_ptr<PhysicsMaterial> CreateMaterial(float StaticFriction = 0.5f, float DynamicFriction = 0.5f, float Restitution = 0.3f);
	void DeleteMaterial(std::shared_ptr<PhysicsMaterial> material);

	std::shared_ptr<StaticPhysicsObject> CreateStaticObject(const PlaneDesc& planeDesc);
	void DeleteStaticObject(std::shared_ptr<StaticPhysicsObject> object);

	std::shared_ptr<RigidBody> CreateRigidBody(const BoxDesc& boxDesc, float mass = 1.0f, const glm::vec3& pos = glm::vec3(0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
    void DeleteRigidBody(std::shared_ptr<RigidBody> object);

	std::shared_ptr < PhysicsJoint> CreateJoint();
    void DeleteJoint(std::shared_ptr < PhysicsJoint> object);

    void ClearScene(bool rigidBodies = true, bool staticObjects = true, bool joints = true);

	glm::vec3 GetGravity() const { return m_gravity; }
	std::list<std::shared_ptr<RigidBody>> GetRigidBodyList() const { return m_rigidBodies; }
	std::list<std::shared_ptr<StaticPhysicsObject>> GetStaticBodyList() const { return m_staticBodies; }
	std::list<std::shared_ptr<PhysicsMaterial>> GetMaterialList() const { return m_materials; }
	std::list<std::shared_ptr<PhysicsJoint>> GetJointList() const { return m_joints; }

	btDiscreteDynamicsWorld* GetWorld() { return m_dynamicsWorld; }
private:
	glm::vec3 m_gravity;
	PhysicsContactCallback m_contactCallback = nullptr;

	btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
	btCollisionDispatcher* m_dispatcher = nullptr;
	btBroadphaseInterface* m_broadPhase = nullptr;
	btSequentialImpulseConstraintSolver* m_solver = nullptr;
	btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;

	std::list<std::shared_ptr<RigidBody>> m_rigidBodies;
	std::list<std::shared_ptr<StaticPhysicsObject>> m_staticBodies;
	std::list<std::shared_ptr<PhysicsMaterial>> m_materials;
	std::list<std::shared_ptr<PhysicsJoint>> m_joints;

};

PhysicsSimulator& GetPhysicsSimulator();