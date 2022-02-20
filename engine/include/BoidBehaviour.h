
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
	virtual glm::vec3 CalculateForce() = 0;
	virtual void SetMembers(glm::vec3* pv_members) = 0;
	BoidForce* next;
	float m_weight;
	static float s_speed;
protected:
	glm::vec3 m_pos;

};

enum ForceDataOffset
{
	POS,
	TARGET,
	FWD,
	UP,
	WANDERPOINT,
	COUNT
};

class Seek : public BoidForce
{
public:
	Seek(bool a_flee) : bFlee(a_flee) {};
	glm::vec3 CalculateForce() override;
	void SetMembers(glm::vec3* pv_members) override
	{
		m_pos = pv_members[ForceDataOffset::POS];
		m_target = pv_members[ForceDataOffset::TARGET];	
	}
	bool bFlee;
	static glm::vec3 CalculateForce(glm::vec3 target, glm::vec3 pos, bool flee);
private:
	glm::vec3 m_target;
};

class Wander : public BoidForce
{
public:
	Wander(float a_length = 2.0f, float a_radius = 1.0f, float a_jitter = 0.5f) : m_length(a_length), m_radius(a_radius), m_jitter(a_jitter) {};
	glm::vec3 CalculateForce() override;
	static glm::vec3 CalculateForce(const glm::vec3& a_pos, const glm::vec3& a_fwd, glm::vec3& a_wanderPoint, const float length, const float radius, const float jitter);
	void SetMembers(glm::vec3* pv_members) override
	{
		m_pos = pv_members[ForceDataOffset::POS];
		m_fwd = pv_members[ForceDataOffset::FWD];
		m_wanderPoint = pv_members[ForceDataOffset::WANDERPOINT];
	}
private:
	glm::vec3 m_wanderPoint;
	glm::vec3 m_fwd;
	float m_length;
	float m_radius;
	float m_jitter;
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
	Transform* p_transform;
public:
	static void SetupForceList();
};


#endif // !BOID_BEHAVIOUR_H

