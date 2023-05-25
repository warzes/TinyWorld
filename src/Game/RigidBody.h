#pragma once

// TODO: отрефакторить объединив с статикобъектом

class RigidBody
{
public:
	~RigidBody();

	void CreateBox(const glm::vec3& boxHalfExtents, float mass, const glm::vec3& pos, const glm::quat& rotation);
	void Destroy();

	void SetGravity(const glm::vec3& gravity);
	void SetMass(float mass);
	void SetAutoSleep(bool sleep);
	void SetMassCenter(const glm::vec3& localPoint);

	void AddVelocity(const glm::vec3& direction);
	void AddImpulse(const glm::vec3& direction, const glm::vec3& pivotPoint);

	glm::vec3 GetMassCenter() const { return m_localInertia; }

	btTransform GetBTTransform() const; // return bullet transform
	glm::mat4 GetMatrix() const;

private:
	btCollisionShape* m_shape = nullptr;
	btRigidBody* m_body = nullptr;
	glm::vec3 m_localInertia = glm::vec3(0.0f);
	float m_mass = 1.0f;
};

using RigidBodyRef = std::shared_ptr<RigidBody>;