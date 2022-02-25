
#include "Component.h"
#include "Transform.h"
#include <glm.hpp>
#include <vector>
#include <string>

#ifndef BOID_BEHAVIOUR_H
#define BOID_BEHAVIOUR_H

class BoidForce
{
public:
	BoidForce(std::string a_name) : m_weight(1.0f),m_name(a_name) {};
	~BoidForce() { delete next; next = nullptr; }
	virtual glm::vec3 CalculateForce(Transform* a_transform, glm::vec3* a_data, std::vector<GameObject*>* a_neighbourPointer) = 0;
	BoidForce* next;

	float m_weight;
	std::string m_name;
};

class Seek : public BoidForce
{
public:
	Seek(bool a_flee) :  m_bFlee(a_flee), BoidForce(a_flee ? "Flee" : "Seek") {};
	glm::vec3 CalculateForce(Transform* a_transform, glm::vec3* a_data, std::vector<GameObject*>* a_neighbourPointer) override;
	static glm::vec3 CalculateForce(glm::vec3 target, glm::vec3 pos, bool flee);
	bool m_bFlee;
};

class Wander : public BoidForce
{
public:
	Wander() : BoidForce("Wander") {};
	glm::vec3 CalculateForce(Transform* a_transform, glm::vec3* a_data, std::vector<GameObject*>* a_neighbourPointer) override;
	static glm::vec3 CalculateForce(const glm::vec3& a_pos, const glm::vec3& a_fwd, glm::vec3& a_wanderPoint,
		const float length, const float radius, const float jitter);
protected:
	static float s_length;
	static float s_radius;
	static float s_jitter;
};

class Flock : public BoidForce
{
public:
	Flock() : BoidForce("Flock") {};
	glm::vec3 CalculateForce(Transform* a_transform, glm::vec3* a_data, std::vector<GameObject*>* a_neighbourPointer) override;
	static float s_neighbourhoodRaidus;
	static float s_weightAlignment;
	static float s_weightSeperation;
	static float s_weightCohesion;
};

enum BoidDataOffset
{
	TARGET,
	WANDER,
	COUNT,
};

class BoidBehaviour : public Component
{
public:
	BoidBehaviour(GameObject* a_owner) : Component(a_owner), m_velocity(glm::vec3(0)) {};
	static BoidForce* s_head;
	void Update(float deltaTime) override;
	void Draw(unsigned int a_shader) override {};
	void Init() override;

	inline void SetNeighbours(std::vector<GameObject*>* a_neighbours){	m_neighbours = a_neighbours;}

	glm::vec3 GetVelocity() { return m_velocity; }
protected:
	glm::vec3 m_velocity;
	static float s_maxVelocity;
	static float s_speed;
	Transform* p_transform;
	glm::vec3 data[BoidDataOffset::COUNT];
	std::vector<GameObject*>* m_neighbours;

	
	void SetTarget(glm::vec3 a_target){data[BoidDataOffset::TARGET] = a_target;}
public:
	static void SetupForceList();
};


#endif // !BOID_BEHAVIOUR_H

