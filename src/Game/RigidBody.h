#pragma once

class RigidBody
{
public:
	void SetGravity(const glm::vec3& gravity);
};

using RigidBodyRef = std::shared_ptr<RigidBody>;