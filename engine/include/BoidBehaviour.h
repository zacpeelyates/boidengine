
#include "Component.h"
#include "Transform.h"
#include <glm.hpp>

#ifndef BOID_BEHAVIOUR_H
#define BOID_BEHAVIOUR_H

class BoidForce
{
public:
	BoidForce() : m_weight(1.0f) {};
	~BoidForce() { delete next; next = nullptr; }
	virtual glm::vec3 CalculateForce(Transform* a_transform, glm::vec3* a_data) = 0;
	BoidForce* next;
	float m_weight;
};



class Seek : public BoidForce
{
public:
	Seek(bool a_flee) : m_bFlee(a_flee) {};
	glm::vec3 CalculateForce(Transform* a_transform, glm::vec3* a_data) override;
	bool m_bFlee;
	static glm::vec3 CalculateForce(glm::vec3 target, glm::vec3 pos, bool flee);
};

class Wander : public BoidForce
{
public:
	Wander() {};
	glm::vec3 CalculateForce(Transform* a_transform, glm::vec3* a_data) override;
	static glm::vec3 CalculateForce(const glm::vec3& a_pos, const glm::vec3& a_fwd, glm::vec3& a_wanderPoint,
		const float length, const float radius, const float jitter);
protected:
	static float s_length;
	static float s_radius;
	static float s_jitter;
};

enum BoidDataOffset
{
	TARGET,
	WANDER,
	COUNT
};

class BoidBehaviour : public Component
{
public:
	BoidBehaviour(GameObject* a_owner) : Component(a_owner), m_velocity(glm::vec3(0)) {};
	static BoidForce* s_head;
	void Update(float deltaTime) override;
	void Draw(unsigned int a_shader) override {};
	void Init() override;
protected:
	glm::vec3 m_velocity;
	static float s_maxVelocity;
	static float s_speed;
	Transform* p_transform;
	glm::vec3 data[BoidDataOffset::COUNT];

	
	void SetTarget(glm::vec3 a_target)
	{
		data[BoidDataOffset::TARGET] = a_target;
	}
public:
	static void SetupForceList();
};


#endif // !BOID_BEHAVIOUR_H

