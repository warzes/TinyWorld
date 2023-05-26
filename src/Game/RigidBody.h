#pragma once

// TODO: отрефакторить объединив с статикобъектом

enum class RigidBodyShapeType
{
	Box,
	Sphere,
	Capsule,
	Cone,
	Convex,
	TriangleMesh
};

class RigidBody
{
public:
	~RigidBody();

	void CreateBox(const glm::vec3& boxHalfExtents, float mass, const glm::vec3& position, const glm::quat& rotation);
	void CreateSphere(float radius, float mass, const glm::vec3& position);
	void CreateCapsule(float radius, float height, float mass, const glm::vec3& position, const glm::quat& rotation);
	void CreateCone(float radius, float height, float mass, const glm::vec3& position, const glm::quat& rotation);
	void CreateConvex(const std::vector<glm::vec3>& points, float mass, const glm::vec3& position, const glm::quat& rotation);
	void CreateTriangleMesh(const std::vector<glm::vec3>& vert, const std::vector<unsigned>& index, float mass, const glm::vec3& position, const glm::quat& rotation);

	void Destroy();

	RigidBodyShapeType GetShapeType() const { return m_shapeType; }

	void SetGravity(const glm::vec3& gravity);
	void SetMass(float mass);
	void SetMassCenter(const glm::vec3& localPoint);
	void SetSleeping(bool sleeping);
	void SetSleepThreshold(float linear, float angular);
	void SetDamping(float linear, float angular);

	void SetMaterial(float restitution, float friction);
	void SetLinearFactor(const glm::vec3& factor);
	void SetAngularFactor(const glm::vec3& factor);

	void SetLinearVelocity(const glm::vec3& velocity);
	void SetAngularVelocity(const glm::vec3& velocity);

	void AddVelocity(const glm::vec3& direction);
	void AddImpulse(const glm::vec3& direction, const glm::vec3& pivotPoint);

	glm::vec3 GetMassCenter() const { return m_localInertia; }

	btTransform GetBTTransform() const; // return bullet transform
	glm::mat4 GetMatrix() const;

private:
	void create(float mass, const glm::vec3& pos, const glm::quat& rotation);
	btCollisionShape* m_shape = nullptr;
	btRigidBody* m_body = nullptr;
	glm::vec3 m_localInertia = glm::vec3(0.0f);
	float m_mass = 1.0f;
	RigidBodyShapeType m_shapeType;
};

using RigidBodyRef = std::shared_ptr<RigidBody>;