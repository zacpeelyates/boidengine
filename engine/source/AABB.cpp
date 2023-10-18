// File: AABB.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Implementation of Axis-Aligned Bounding-Box

#include "AABB.h"


void AABB::Translate(glm::vec3 a_delta)
{
	m_minPoint += a_delta;
	m_maxPoint += a_delta;
}

auto AABB::Intersects(const AABB* a_other) const -> bool
{
	if (m_maxPoint.x < a_other->m_minPoint.x || m_minPoint.x > a_other->m_maxPoint.x)
	{
		return false;
	}
	if (m_maxPoint.y < a_other->m_minPoint.y || m_minPoint.y > a_other->m_maxPoint.y)
	{
		return false;
	}
	if (m_maxPoint.z < a_other->m_minPoint.z || m_minPoint.z > a_other->m_maxPoint.z)
	{
		return false;
	}
	return true;
}

auto AABB::Contains(glm::vec3 a_other) const -> bool
{
	if (m_maxPoint.x < a_other.x || m_minPoint.x > a_other.x)
	{
		return false;
	}
	if (m_maxPoint.y < a_other.y || m_minPoint.y > a_other.y)
	{
		return false;
	}
	if (m_maxPoint.z < a_other.z || m_minPoint.z > a_other.z)
	{
		return false;
	}
	return true;
}

void AABB::IncreaseBounds(const float a_value)
{
	auto delta = glm::vec3(a_value);
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
	auto delta = glm::vec3(a_radius);
	m_minPoint = centre - delta;
	m_maxPoint = centre + delta;
}

void AABB::SetOrigin(glm::vec3 a_origin)
{
	Translate(a_origin - GetCentre());
}

void AABB::GetPoints(glm::vec3* result) const
{
	result[0] = m_minPoint;
	result[1] = glm::vec3(m_minPoint.x, m_minPoint.y, m_maxPoint.z);
	result[2] = glm::vec3(m_minPoint.x, m_maxPoint.y, m_maxPoint.z);
	result[3] = m_maxPoint;

	result[4] = glm::vec3(m_maxPoint.x, m_maxPoint.y, m_minPoint.z);
	result[5] = glm::vec3(m_maxPoint.x, m_minPoint.y, m_minPoint.z);

	result[6] = glm::vec3(m_maxPoint.x, m_minPoint.y, m_maxPoint.z);
	result[7] = glm::vec3(m_minPoint.x, m_maxPoint.y, m_minPoint.z);
}
