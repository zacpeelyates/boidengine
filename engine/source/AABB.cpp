#include "AABB.h"

AABB::AABB(const glm::vec3 a_origin, const float a_size)
{
	SetBounds(a_origin, a_size);
}

void AABB::Translate(glm::vec3 a_delta)
{
	m_minPoint += a_delta;
	m_maxPoint += a_delta;
}

bool AABB::Intersects(AABB* a_other)
{
	if (m_maxPoint.x < a_other->m_minPoint.x || m_minPoint.x > a_other->m_maxPoint.x) return false;
	if (m_maxPoint.y < a_other->m_minPoint.y || m_minPoint.y > a_other->m_maxPoint.y) return false;
	if (m_maxPoint.z < a_other->m_minPoint.z || m_minPoint.z > a_other->m_maxPoint.z) return false;
	return true;
}

void AABB::IncreaseBounds(const float a_value)
{
	glm::vec3 delta = glm::vec3(a_value);
	m_minPoint -= delta;
	m_maxPoint += delta;
}

void AABB::IncreaseBounds(const glm::vec3 a_v3)
{
	m_minPoint -= a_v3;
	m_maxPoint += a_v3;
}

void AABB::SetBounds(const glm::vec3 a_point, float a_radius)
{
	m_minPoint = a_point - a_radius;
	m_maxPoint = a_point + a_radius;
}

void AABB::SetRadius(float a_radius)
{
	glm::vec3 centre = GetCentre();
	glm::vec3 delta = glm::vec3(a_radius);
	m_minPoint = centre - delta;
	m_maxPoint = centre + delta;
}

void AABB::SetOrigin(glm::vec3 a_origin)
{
	Translate(a_origin - GetCentre());
}


