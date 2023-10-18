// File: BoidBehaviour.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Implements all behaviours and forces that act upon our boids

#include "BoidBehaviour.h"
#include "AABB.h"
#include "GameObject.h"
#include "Gizmos.h"
#include "Octree.h"
#include <ext.hpp>

#define OOB_WEIGHT 5
float BoidBehaviour::s_speed = 0.1F;
float BoidBehaviour::s_maxVelocity = 1.0F;
AABB BoidBehaviour::s_bounds = AABB(glm::vec3(100));
BoidForce* BoidBehaviour::s_head = nullptr;
float Wander::s_jitter = 1.0F;
float Wander::s_radius = 2.0F;
float Wander::s_length = 3.0F;
float Flock::s_neighbourhoodRaidus = 25.0F;
float Flock::s_weightAlignment = 1.0F;
float Flock::s_weightCohesion = 1.0F;
float Flock::s_weightSeperation = 1.0F;
glm::vec3 BoidBehaviour::seekPoint = glm::vec3(0);
glm::vec3 BoidBehaviour::fleePoint = glm::vec3(0);


void BoidBehaviour::Init()
{
	p_transform = m_owner->GetComponent<Transform>().get(); //fake warning honest
}

void BoidBehaviour::UpdateBoidNeighbourLists()
{
	for (GameObject* g : GameObject::s_GameObjects)
	{
		BoidBehaviour* b = g->GetComponent<BoidBehaviour>().get(); //again im pretty sure this warning is wrong -- it all seems to work
		if (b->b_hasUpdatedNeighbours) continue; //early out

		auto octant = Octree::root->GetObjects(g);
		if (octant == nullptr) return; //early out
		b->SetNeighbours(octant);

		for (GameObject* n : *octant)
		{
			//we can update neighbours for all objects in this octant
			n->GetComponent<BoidBehaviour>()->SetNeighbours(octant);
		}
	}
	
}

void BoidBehaviour::SetupForceList()
{
	//initialize list of forces in priority order
	const std::vector<BoidForce*> forces = {new Flock(), new Seek(1.0, false), new Seek(1.0, true), new Wander(1.0F)};
	s_head = *forces.begin();
	for (unsigned int i = 1; i < forces.size(); ++i)
	{
		forces[i]->prev = forces[i - 1];
		forces[i - 1]->next = forces[i];
	}
	seekPoint = linearRand(s_bounds.GetMin(), s_bounds.GetMax());
	fleePoint = linearRand(s_bounds.GetMin(), s_bounds.GetMax());
}

void BoidBehaviour::CreatePartitions()
{
	//divide bounds into octree
	Octree::Create(&s_bounds, 1);
	Octree::PartitionObjects(GameObject::s_GameObjects, Octree::root);
	UpdateBoidNeighbourLists();
}


void BoidBehaviour::Update(float deltaTime)
{
	m_velocity *= 1 - deltaTime * 0.1F; //dampen forces from earlier frames
	//total forces for this frame
	glm::vec3 totalForce(0); //force to calculate 

	for (BoidForce* current = s_head; current != nullptr; current = current->next)
	{
		if (current->m_enabled && length(totalForce) < s_maxVelocity)
		{
			totalForce += current->CalculateForce(p_transform, wanderPoint, m_neighbours) * current->m_weight;
		}
	}

	glm::vec3 pos = p_transform->GetRow(POS_VEC);

	if (!s_bounds.Contains(pos))
	{
		totalForce += Seek::CalculateForce(glm::vec3(0, 0, 0), pos) * OOB_WEIGHT;
		//encourage boids back towards centre if they are out of bounds. Uses static implementation in order to ignore weight of Seek set by user.
	}

	glm::vec3 clamp(s_maxVelocity);
	m_velocity += glm::clamp(totalForce * deltaTime * s_speed, -clamp, clamp);
	glm::vec3 nextPos = pos + m_velocity;


	if (length(m_velocity) == 0.0f)
	{
		return; //no velocity so no changes this frame
	}

	glm::vec3 fwd = normalize(m_velocity);
	Gizmos::addLine(nextPos, nextPos + fwd, glm::vec4(0.8f));
	glm::vec3 right = cross(fwd, p_transform->GetRow(UP_VEC));


	//update matrix
	p_transform->SetRow(POS_VEC, nextPos);
	p_transform->SetRow(FWD_VEC, fwd);
	p_transform->SetRow(RIGHT_VEC, right);

	b_hasUpdatedNeighbours = false;
}

//////////////////////////////// Seek/Flee ///////////////////////////////////////////////////////

BoidForce::~BoidForce()
{
	delete next;
	next = nullptr;
}

auto Seek::CalculateForce(Transform* a_transform, glm::vec3 /*wanderPoint*/, std::vector<GameObject*>* /*a_neighbourPointer*/)-> glm::vec3 //Seek / Flee force

{
	return CalculateForce(m_bFlee ? BoidBehaviour::fleePoint : BoidBehaviour::seekPoint, a_transform->GetRow(POS_VEC),
	                      m_bFlee);
}

auto Seek::CalculateForce(glm::vec3 target, glm::vec3 pos, bool flee) -> glm::vec3 //static implementation

{
	glm::vec3 dir(target - pos);
	if (length(dir) == 0.0f)
	{
		return dir; //early out
	}
	return normalize(dir) * (flee ? -1 : 1); //invert if fleeing
}

//////////////////////////////// Wander ///////////////////////////////////////////////////////

auto Wander::CalculateForce(Transform* a_transform, glm::vec3 wanderPoint, std::vector<GameObject*>*) -> glm::vec3

{
	return CalculateForce(a_transform->GetRow(POS_VEC), a_transform->GetRow(FWD_VEC), wanderPoint, s_length, s_radius,
	                      s_jitter);
}

auto Wander::CalculateForce(const glm::vec3& pos, const glm::vec3& fwd, glm::vec3& wanderPoint,const float length, const float radius, const float jitter) -> glm::vec3
//static implementation
{
	glm::vec3 origin = pos + fwd * length;

	if (glm::length(wanderPoint) == 0.0F)
	{
		wanderPoint = origin + glm::sphericalRand(radius);
		//make a wanderpoint if we dont have one (should only be called on first pass)
	}
	if (wanderPoint == origin)
	{
		return glm::vec3(0);
	}
	glm::vec3 dir = normalize(wanderPoint - origin);
	wanderPoint = origin + dir * radius + glm::sphericalRand(jitter);
	Gizmos::addLine(pos, origin, glm::vec4(200, 50, 50, 200));
	Gizmos::addLine(origin, wanderPoint, glm::vec4(180, 40, 40, 200));
	Gizmos::addBox(wanderPoint, glm::vec3(radius), true, glm::vec4(200));

	return Seek::CalculateForce(wanderPoint, pos, false);
}

//////////////////////////////// Flocking ///////////////////////////////////////////////////////

auto Flock::CalculateForce(Transform* a_transform, glm::vec3 /*wanderPoint*/,std::vector<GameObject*>* a_neighbourPointer) -> glm::vec3
{
	if (a_neighbourPointer == nullptr) return glm::vec3(0); //early out
	unsigned int neighbourCount = 0;
	glm::vec3 localPos = a_transform->GetRow(POS_VEC);
	glm::vec3 fwdVec = a_transform->GetRow(FWD_VEC);

	glm::vec3 seperation(0);
	glm::vec3 alignment(0);
	glm::vec3 cohesion(0);

	for (GameObject* other : *a_neighbourPointer)
	{
		Transform* otherT = other->GetComponent<Transform>().get();
		//visual studio thinks this pointer is dangling but i disagree
		if (otherT == a_transform)
		{
			continue; //we are looking at ourselves get out early
		}
		glm::vec3 otherPos = otherT->GetRow(POS_VEC);
		glm::vec3 toOther = otherPos - localPos;


		if (dot(fwdVec, normalize(toOther)) < 0.7F /* look angle */ && length(toOther) < s_neighbourhoodRaidus)
		{
			//boid is in our neighbourhood
			neighbourCount++;
			seperation += toOther * -1; //toLocal		
			cohesion += otherPos;
			alignment += other->GetComponent<BoidBehaviour>()->GetVelocity();
		}
	}
	if (neighbourCount == 0)
	{
		return glm::vec3(0);
	}

	for (glm::vec3 v : {seperation, alignment, cohesion})
	{
		v /= neighbourCount;
		if (length(v) != 0.0f)
		{
			v = normalize(v);
		}
	}

	return seperation + alignment + cohesion; //total our forces
}
