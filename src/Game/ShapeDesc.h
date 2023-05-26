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
	float radius = 0.5f;
	float height = 1.5f;
};

struct ConeDesc
{
	float radius = 0.5f;
	float height = 1.5f;
};

struct ConvexDesc
{
	std::vector<glm::vec3> points;
};

struct TriangleMeshDesc
{
	std::vector<glm::vec3> verts;
	std::vector<unsigned> indeces;
};