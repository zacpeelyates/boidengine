#include "BoidBehaviour.h"
#include "GameObject.h"
#include <ext.hpp>

float BoidBehaviour::s_speed = 1.0f;
float BoidBehaviour::s_maxVelocity =3.0f;
BoidForce* BoidBehaviour::s_head = nullptr;
float Wander::s_jitter = 1.0f;
float Wander::s_radius = 2.0f;
float Wander::s_length = 3.0f;
float Flock::s_neighbourhoodRaidus = 4.0f;
float Flock::s_weightAlignment = 1.2f;
float Flock::s_weightCohesion = 0.5f;
float Flock::s_weightSeperation = 0.8f;

void BoidBehaviour::Init()
{
	p_transform = m_owner->GetComponent<Transform>().get(); //fake warning honest
	data[BoidDataOffset::TARGET] = glm::vec3(0,0,0);
	data[BoidDataOffset::WANDER] = glm::vec3(0);
	
	m_neighbours = &GameObject::s_GameObjects;
	//todo update this to use octals
}

void BoidBehaviour::SetupForceList()
{
	//todo make this not stupid LOL
	if (s_head == nullptr) {
		s_head = new Seek(false); //seek
		s_head->next = new Seek(true); //flee
		s_head->next->next = new Wander(); //wander
		s_head->next->next->next = new Flock(); //flocking
	}
}	

void BoidBehaviour::Update(float deltaTime)
{
	//total forces for this frame
	glm::vec3 totalForce(0); //calculate 

	for (BoidForce* current = s_head; current != nullptr; current = current->next)
	{
		totalForce += current->CalculateForce(p_transform,data,m_neighbours) * current->m_weight;
	}

	glm::vec3 clamp(s_maxVelocity, 0, s_maxVelocity);
	m_velocity += glm::clamp(totalForce * deltaTime * s_speed, -clamp, clamp ) ;

	if (m_velocity == glm::vec3(0)) return; //no velocity so no changes this frame
	
	glm::vec3 fwd = glm::normalize(m_velocity);
	glm::vec3 right = glm::cross(fwd, p_transform->GetRow(UP_VEC));
	glm::vec3 nextPos = p_transform->GetRow(MATRIX_ROW::POS_VEC) + m_velocity;

	//update matrix
	p_transform->SetRow(MATRIX_ROW::POS_VEC, nextPos);
	p_transform->SetRow(MATRIX_ROW::FWD_VEC, fwd);
	p_transform->SetRow(MATRIX_ROW::RIGHT_VEC, right);

}

//////////////////////////////// Seek/Flee ///////////////////////////////////////////////////////

glm::vec3 Seek::CalculateForce(Transform* a_transform, glm::vec3* a_data, std::vector<GameObject*>* a_neighbourPointer) //Seek / Flee force
{
	return CalculateForce(a_data[BoidDataOffset::TARGET], a_transform->GetRow(MATRIX_ROW::POS_VEC), m_bFlee);
}

glm::vec3 Seek::CalculateForce(glm::vec3 target, glm::vec3 pos, bool flee) //static implementation
{
	glm::vec3 dir(target-pos);
	if (glm::length(dir) == 0) return dir; //early out
	return glm::normalize(dir) * (flee ? -1 : 1); //invert if fleeing
}

//////////////////////////////// Wander ///////////////////////////////////////////////////////

glm::vec3 Wander::CalculateForce(Transform* a_transform, glm::vec3* a_data, std::vector<GameObject*>* a_neighbourPointer) //wander force
{
	return CalculateForce(a_transform->GetRow(MATRIX_ROW::POS_VEC), a_transform->GetRow(MATRIX_ROW::FWD_VEC),
											  a_data[BoidDataOffset::WANDER], s_length, s_radius, s_jitter);
}

glm::vec3 Wander::CalculateForce(const glm::vec3& pos,const glm::vec3& fwd, glm::vec3& wanderPoint, 
	const float length, const float radius, const float jitter) //static implementation
{

	glm::vec3 origin = pos + (fwd * length);
	if (glm::length(wanderPoint) == 0.0f) wanderPoint = origin + glm::sphericalRand(radius); //make a wanderpoint if we dont have one (should only be called on first pass)
	glm::vec3 dir = glm::normalize(wanderPoint - origin);
	wanderPoint = origin + (dir * radius) + glm::sphericalRand(jitter);

	return Seek::CalculateForce(wanderPoint, pos, false);
}

//////////////////////////////// Flocking ///////////////////////////////////////////////////////

glm::vec3 Flock::CalculateForce(Transform* a_transform, glm::vec3* a_data, std::vector<GameObject*>* a_neighbourPointer)
{
	unsigned int neighbourCount = 0;

	glm::vec3 seperation(0);
	glm::vec3 alignment(0);
	glm::vec3 cohesion(0);

	glm::vec3 localPos = a_transform->GetRow(MATRIX_ROW::POS_VEC);

	for (GameObject* g : *a_neighbourPointer)
	{
		Transform* otherT = g->GetComponent<Transform>().get();
		if (otherT == a_transform) break; //we are looking at ourselves get out early
		glm::vec3 otherPos = otherT->GetRow(MATRIX_ROW::POS_VEC);

		if (glm::length(localPos - otherPos) < s_neighbourhoodRaidus)
		{
			//boid is in our neighbourhood
			neighbourCount++;
			seperation += localPos - otherPos;
			cohesion += otherPos;
			alignment += g->GetComponent<BoidBehaviour>().get()->GetVelocity();	
		}
	}

	if (neighbourCount == 0) return glm::vec3(0);

	seperation = glm::normalize(seperation / neighbourCount) * s_weightSeperation;
	alignment /= glm::normalize(alignment / neighbourCount) * s_weightAlignment;
	cohesion /= glm::normalize (cohesion/neighbourCount - localPos) * s_weightCohesion;

	return seperation + alignment + cohesion;

}
