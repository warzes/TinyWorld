#pragma once

const float STEP_HEIGHT = 0.4f;
const float JUMP_HEIGHT = 2.0f;
const float FALL_SPEED = 55.0f;
const float JUMP_SPEED = 14.0f;
const float MAX_SLOPE = 45.0f;
const float DEFAULT_DAMPING = 0.2f;
const btVector3 KINEMATIC_GRAVITY(0.0f, -14.0f, 0.0f);

class CharacterController
{
public:
	CharacterController();
	~CharacterController();

	void Warp(const glm::vec3& positions);

	void Walk(const glm::vec3& directions);
	void Jump();

	bool IsGround() const;

	btTransform GetBTTransform() const; // return bullet transform
	glm::mat4 GetMatrix() const;
	glm::vec3 GetPosition() const;

private:
	btPairCachingGhostObject* m_pairCachingGhostObject = nullptr;
	btKinematicCharacterController* m_kinematicCharacterController = nullptr;
	btConvexShape* m_capsuleShape = nullptr;

	btVector3 m_gravity = KINEMATIC_GRAVITY;

	float m_maxJumpHeight = JUMP_HEIGHT;
	float m_jumpSpeed = JUMP_SPEED;
	float m_fallSpeed = FALL_SPEED;
	float m_stepHeight = STEP_HEIGHT;
	float m_maxSlope = MAX_SLOPE;
	float m_linearDamping = DEFAULT_DAMPING;
	float m_angularDamping = DEFAULT_DAMPING;

};

using CharacterControllerRef = std::shared_ptr<CharacterController>;