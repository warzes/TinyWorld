#pragma once

#include "ShapeDesc.h"

class StaticPhysicsObject
{
public:
	~StaticPhysicsObject();

	void CreatePlane(const glm::vec3& planeNormal = glm::vec3(0.0f, 1.0f, 0.0f), float planeConstant = 0.0f);
	void CreateConvex(const std::vector<glm::vec3>& points, const glm::vec3& position, const glm::quat& rotation);
	void CreateTriangleMesh(const std::vector<glm::vec3>& vert, const std::vector<unsigned>& index, const glm::vec3& position, const glm::quat& rotation);
	void Destroy();

	PhysicsObjectShapeType GetShapeType() const { return m_shapeType; }

	btTransform GetBTTransform() const; // return bullet transform
	glm::mat4 GetMatrix() const;

private:
	void create(const glm::vec3& pos, const glm::quat& rotation);
	btCollisionShape* m_shape = nullptr;
	btRigidBody* m_body = nullptr;

	PhysicsObjectShapeType m_shapeType;
};

using StaticPhysicsObjectRef = std::shared_ptr<StaticPhysicsObject>;