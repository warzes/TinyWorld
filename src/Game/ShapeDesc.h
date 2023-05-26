#pragma once

struct PlaneDesc
{
	glm::vec3 planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
	float planeConstant = 0.0f;
};

struct BoxDesc
{
	const glm::vec3& boxHalfExtents = glm::vec3(0.5f, 0.5f, 0.5f);
};

struct SphereDesc
{
	float radius = 1.0f;
};

struct CapsuleDesc
{

};

struct TriangleMeshDesc
{

};