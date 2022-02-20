#include "BoidBehaviour.h"
#include "GameObject.h"
#include <ext.hpp>

float BoidForce::s_speed = 1.0f;
BoidForce* BoidBehaviour::s_head = nullptr;

void BoidBehaviour::Init()
{
	p_transform = m_owner->GetComponent<Transform>().get();
}

void BoidBehaviour::SetupForceList()
{
	if (s_head == nullptr) {
		s_head = new Seek(false);
		s_head->next = new Seek(true);
		s_head->next->next = new Wander();
	}
}

void BoidBehaviour::Update(float deltaTime)
{
	glm::vec3 target(10, 10, 10);
	glm::vec3 data[ForceDataOffset::COUNT];
	data[ForceDataOffset::POS] = p_transform->GetRow(MATRIX_ROW::POS_VEC);
	data[ForceDataOffset::TARGET] = target;
	data[ForceDataOffset::FWD] = p_transform->GetRow(MATRIX_ROW::FWD_VEC);
	data[ForceDataOffset::UP] = p_transform->GetRow(MATRIX_ROW::UP_VEC);
	data[ForceDataOffset::WANDERPOINT] = glm::vec3(0);
	glm::vec3 totalForce(0);
	for (BoidForce* current = s_head; current != nullptr; current = current->next)
	{
		current->SetMembers(data);
		totalForce += current->CalculateForce() * current->m_weight;
	}

	m_velocity = totalForce - m_velocity;

	glm::vec3 pos = p_transform->GetRow(MATRIX_ROW::POS_VEC);
	
	pos += m_velocity * deltaTime;

	glm::vec3 fwd = m_velocity.length() > 0.0f ? glm::normalize(m_velocity) : m_velocity;
	glm::vec3 right = glm::cross(fwd.length() > 0 ? fwd : p_transform->GetRow(MATRIX_ROW::FWD_VEC), p_transform->GetRow(MATRIX_ROW::UP_VEC));
	
	//update transform
	p_transform->SetRow(MATRIX_ROW::FWD_VEC, fwd);
	p_transform->SetRow(MATRIX_ROW::RIGHT_VEC, right);
	p_transform->SetRow(MATRIX_ROW::POS_VEC, pos);
	//p_transform->SetRow(MATRIX_ROW::UP_VEC, up);
}

glm::vec3 Seek::CalculateForce() //Seek / Flee force
{
	glm::vec3 dir(m_pos - m_target);
	if (dir.length() == 0) return dir; //early out
	return glm::normalize(dir) * s_speed * (bFlee ? -1 : 1); //invert if fleeing
}

glm::vec3 Seek::CalculateForce(glm::vec3 target, glm::vec3 pos, bool flee) //static implementation
{
	glm::vec3 dir(pos - target);
	if (dir.length() == 0) return dir; //early out
	return glm::normalize(dir) * s_speed * (flee ? -1 : 1); //invert if fleeing
}

glm::vec3 Wander::CalculateForce()
{
	glm::vec3 origin = m_pos + (m_fwd * m_length);
	glm::vec3 dir = glm::normalize(m_wanderPoint - origin) * m_radius;
	m_wanderPoint = origin + dir + glm::sphericalRand(m_jitter);

	return Seek::CalculateForce(m_wanderPoint,m_pos,false);
}

glm::vec3 Wander::CalculateForce(const glm::vec3& pos,const glm::vec3& fwd, glm::vec3& wanderPoint, const float length, const float radius, const float jitter) //static implementation
{
	glm::vec3 origin = pos + (fwd * length);
	glm::vec3 dir = glm::normalize(wanderPoint - origin) * radius;
	wanderPoint = origin + dir + glm::sphericalRand(jitter);

	return Seek::CalculateForce(wanderPoint, pos, false);
}
