#pragma once

class StaticPhysicsObject
{
public:
	~StaticPhysicsObject();

	void CreatePlane(const glm::vec3& planeNormal = glm::vec3(0.0f, 1.0f, 0.0f), float planeConstant = 0.0f);
	void Destroy();

	btTransform GetBTTransform() const; // return bullet transform
	glm::mat4 GetMatrix() const;

private:
	btCollisionShape* m_shape = nullptr;
	btRigidBody* m_body = nullptr;
};

using StaticPhysicsObjectRef = std::shared_ptr<StaticPhysicsObject>;