
#include <glm.hpp>

class AABB //Axis-Aligned Bounding Box
{
public: 
	AABB() : m_minPoint(glm::vec3(0)), m_maxPoint(glm::vec3(0)) {};
	AABB(const glm::vec3 a_minPoint, const glm::vec3 a_maxPoint) : m_minPoint(a_minPoint), m_maxPoint(a_maxPoint) {}; //AABB with given min/max points
	AABB(const glm::vec3 a_origin, const float a_size = .0f); //AABB of given size around given origin point
	~AABB() {};

	inline glm::vec3 GetMin() const { return m_minPoint; };
	inline glm::vec3 GetMax() const { return m_maxPoint; };

	inline glm::vec3 GetDiagonal() const{ return m_maxPoint - m_minPoint; }
	inline glm::vec3 GetCentre() const { return m_minPoint + (GetDiagonal() * 0.5f); }

	void Translate(const glm::vec3 a_delta);

	bool Intersects(AABB* a_other);
	
	void IncreaseBounds(const float a_value);
	void IncreaseBounds(const glm::vec3 a_v3);

	void SetBounds(const glm::vec3 a_point, float a_radius);
	void SetRadius(float a_radius);

	void SetOrigin(glm::vec3 a_origin);

	
private:
	glm::vec3 m_minPoint;
	glm::vec3 m_maxPoint;
};