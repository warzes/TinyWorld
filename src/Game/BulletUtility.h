#pragma once

template <class T, class F> inline T btCast(const F& from);

template<>
inline glm::vec3 btCast(const btVector3& vec)
{
	return { vec.x(), vec.y(), vec.z() };
}

template<>
inline btVector3 btCast(const glm::vec3& point)
{
	return { point.x, point.y, point.z };
}

template<>
inline glm::quat btCast(const btQuaternion& quat)
{
	return { quat.w(), quat.x(), quat.y(), quat.z() };
}

template<>
inline btQuaternion btCast(const glm::quat& quat)
{
	return btQuaternion(quat.x, quat.y, quat.z, quat.w);
}

inline glm::mat4 btScalarTomat4(btScalar* matrix) 
{
	return glm::mat4(
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15]);
}
