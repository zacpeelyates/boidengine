// File: BoidBehaviour.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Declarations for Boid Behaviour and its requirements (BoidForce, ForceParams, BoidBehaviour)



#ifndef BOID_BEHAVIOUR_H
#define BOID_BEHAVIOUR_H


#include "Component.h"
#include "Transform.h"
#include <glm.hpp>
#include <vector>
#include <string>

struct ForceParams
{
	ForceParams(std::string a_name, float* a_value, float a_min, float a_max) : name(std::move(a_name)), value(a_value), min(a_min), max(a_max) {};
	std::string name;
	float* value;
	float min;
	float max;
};


class BoidForce
{
public:
	BoidForce(int a_weight,std::string a_name) : m_enabled(true),m_weight(a_weight),m_name(a_name) {};
	~BoidForce();
	virtual glm::vec3 CalculateForce(Transform* a_transform, glm::vec3 wanderPoint, std::vector<GameObject*>* a_neighbourPointer) = 0;
	
	BoidForce* next;
	BoidForce* prev;
	bool m_enabled;
	float m_weight;
	std::string m_name;
	const ForceParams* params;
	int numParams = 0;

};

class Seek : public BoidForce
{
public:
	Seek(int a_weight, bool a_flee) : BoidForce(a_weight, a_flee ? "Flee" : "Seek"), m_bFlee(a_flee){};
	glm::vec3 CalculateForce(Transform* a_transform, glm::vec3 wanderPoint, std::vector<GameObject*>* a_neighbourPointer) override;
	static glm::vec3 CalculateForce(glm::vec3 target, glm::vec3 pos, bool flee = false);
	bool m_bFlee;
};

class Wander : public BoidForce
{
public:
	Wander(int a_weight) : BoidForce(a_weight, "Wander")
	{
		params = new ForceParams[]{ ForceParams("Wander Length",&s_length,0.5f,10.0f), ForceParams("Wander Radius",&s_radius,0.5f,10.0f),ForceParams("Wander Jitter",&s_jitter,0.1f,10.0f) };
		numParams = 3;
	};
	glm::vec3 CalculateForce(Transform* a_transform, glm::vec3 wanderPoint, std::vector<GameObject*>* a_neighbourPointer) override;
	static glm::vec3 CalculateForce(const glm::vec3& a_pos, const glm::vec3& a_fwd, glm::vec3& a_wanderPoint, float length, float radius, float jitter);
protected:
	static float s_length;
	static float s_radius;
	static float s_jitter;
};

class Flock : public BoidForce
{
public:
	Flock() : BoidForce(1.0, "Flock") { 
		params = new ForceParams[]{ ForceParams("Alignment",&s_weightAlignment,0.0f,2.0f),ForceParams("Seperation",&s_weightSeperation,0.0f,2.0f),ForceParams("Cohesion",&s_weightCohesion,0.0f,2.0f)};
		numParams = 3;
	};
	glm::vec3 CalculateForce(Transform* a_transform, glm::vec3 wanderPoint, std::vector<GameObject*>* a_neighbourPointer) override;
	static float s_neighbourhoodRaidus;
	static float s_weightAlignment;
	static float s_weightSeperation; 
	static float s_weightCohesion; 
};


class AABB;

class BoidBehaviour : public Component
{
public:
	BoidBehaviour(GameObject* a_owner) : Component(a_owner), m_velocity(glm::vec3(0)) { m_neighbours = nullptr; b_hasUpdatedNeighbours = false; }
	static BoidForce* s_head;

	void Update(float deltaTime) override;
	void Draw(unsigned int a_shader) override {};
	void Init() override;

	void SetNeighbours(std::vector<GameObject*>* a_neighbours) { m_neighbours = a_neighbours; b_hasUpdatedNeighbours = true; }
	glm::vec3 GetVelocity() const { return m_velocity; }
	static void UpdateBoidNeighbourLists();
	static glm::vec3 seekPoint;
	static glm::vec3 fleePoint;
protected:
	glm::vec3 m_velocity;
	static float s_maxVelocity;
	static float s_speed;
	static AABB s_bounds;

	Transform* p_transform;
	glm::vec3 wanderPoint;
	std::vector<GameObject*>* m_neighbours;
	bool b_hasUpdatedNeighbours;
	

	
public:
	static void SetupForceList();
	static void CreatePartitions();
};


#endif // !BOID_BEHAVIOUR_H

