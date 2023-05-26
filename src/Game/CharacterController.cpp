#include "stdafx.h"
#include "CharacterController.h"
#include "BulletUtility.h"
#include "PhysicsSimulator.h"
//-----------------------------------------------------------------------------
CharacterController::CharacterController()
{
	assert(GetPhysicsSimulator().GetWorld());

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(1, 0, 0, 1));

	m_capsuleShape = new btCapsuleShape(0.5f, 1.5f);

	m_pairCachingGhostObject = new btPairCachingGhostObject();
	m_pairCachingGhostObject->setWorldTransform(startTransform);
	m_pairCachingGhostObject->setCollisionShape(m_capsuleShape);
	m_pairCachingGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	m_kinematicCharacterController = new btKinematicCharacterController(m_pairCachingGhostObject, m_capsuleShape, m_stepHeight, { 0.0f, 1.0f, 0.0f });
	m_kinematicCharacterController->setGravity(m_gravity);
	m_kinematicCharacterController->setMaxJumpHeight(m_maxJumpHeight);
	m_kinematicCharacterController->setJumpSpeed(m_jumpSpeed);
	m_kinematicCharacterController->setFallSpeed(m_fallSpeed);
	m_kinematicCharacterController->setStepHeight(m_stepHeight);
	m_kinematicCharacterController->setMaxSlope(glm::radians(m_maxSlope));
	m_kinematicCharacterController->setLinearDamping(m_linearDamping);
	m_kinematicCharacterController->setAngularDamping(m_angularDamping);

	GetPhysicsSimulator().GetWorld()->addCollisionObject(m_pairCachingGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter/*btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter*/);
	GetPhysicsSimulator().GetWorld()->addAction(m_kinematicCharacterController);
}
//-----------------------------------------------------------------------------
CharacterController::~CharacterController()
{
	GetPhysicsSimulator().GetWorld()->removeAction(m_kinematicCharacterController);
	GetPhysicsSimulator().GetWorld()->removeCollisionObject(m_pairCachingGhostObject);
	delete m_kinematicCharacterController;
	delete m_pairCachingGhostObject;
	delete m_capsuleShape;
}
//-----------------------------------------------------------------------------
void CharacterController::Warp(const glm::vec3& positions)
{
	m_kinematicCharacterController->warp({ positions.x, positions.y, positions.z });
}
//-----------------------------------------------------------------------------
void CharacterController::Walk(const glm::vec3& directions)
{
	m_kinematicCharacterController->setWalkDirection({ directions.x, directions.y, directions.z });
}
//-----------------------------------------------------------------------------
void CharacterController::Jump(const glm::vec3& v)
{
	m_kinematicCharacterController->jump({v.x, v.y, v.z});
}
//-----------------------------------------------------------------------------
bool CharacterController::IsGround() const
{
	return m_kinematicCharacterController->onGround();
}
//-----------------------------------------------------------------------------
btTransform CharacterController::GetBTTransform() const
{
	return m_pairCachingGhostObject->getWorldTransform();
}
//-----------------------------------------------------------------------------
glm::mat4 CharacterController::GetMatrix() const
{
	// чтобы не создавать кучу временных объектов надо просто написать свой MotionState вместо юзанья btDefaultMotionState
	// также возможно в getOpenGLMatrix можно передать саму glm::mat4 скастовав в флоаты
	static btScalar mat[16];
	GetBTTransform().getOpenGLMatrix(mat);
	return btScalarTomat4(mat);
}
//-----------------------------------------------------------------------------
glm::vec3 CharacterController::GetPosition() const
{
	const btVector3 position = GetBTTransform().getOrigin();
	return { position.x(), position.y(), position.z() };
}
//-----------------------------------------------------------------------------