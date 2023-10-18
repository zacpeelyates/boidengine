// File: AABB.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Declarations for Axis Aligned Bounding Box

#ifndef AABB_H
#define AABB_H
#include <glm.hpp>

class AABB //Axis-Aligned Bounding Box
{
public:
	AABB() : m_minPoint(glm::vec3(0)), m_maxPoint(glm::vec3(0))
	{
	}

	AABB(const glm::vec3 a_minPoint, const glm::vec3 a_maxPoint) : m_minPoint(a_minPoint), m_maxPoint(a_maxPoint)
	{
	} //AABB with given min/max points
	explicit AABB(const glm::vec3 a_bounds) : m_minPoint(-a_bounds), m_maxPoint(a_bounds)
	{
	} //Cube AABB about origin from given bound/maxpoint
	AABB(const glm::vec3 a_origin, float a_size)
	{
		SetOrigin(a_origin);
		SetRadius(a_size);
	}

	~AABB() = default;

	auto GetMin() const -> glm::vec3 { return m_minPoint; }
	auto GetMax() const -> glm::vec3 { return m_maxPoint; }

	auto GetDiagonal() const -> glm::vec3 { return m_maxPoint - m_minPoint; }
	auto GetCentre() const -> glm::vec3 { return m_minPoint + GetDiagonal() * 0.5F; }
	auto GetDimensions() const -> glm::vec3 { return m_maxPoint - GetCentre(); }

	void Translate(glm::vec3 a_delta);

	auto Intersects(const AABB* a_other) const -> bool;
	auto Contains(glm::vec3 a_point) const -> bool;

	void IncreaseBounds(float a_value);
	void IncreaseBounds(glm::vec3 a_v3);

	void SetBounds(glm::vec3 a_point, float a_radius);
	void SetRadius(float a_radius);

	void SetOrigin(glm::vec3 a_origin);

	void GetPoints(glm::vec3* a_vec) const;


private:
	glm::vec3 m_minPoint{};
	glm::vec3 m_maxPoint{};
};
#endif //AABB_H
