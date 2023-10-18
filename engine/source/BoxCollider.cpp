// File: BoxCollider.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: BoxCollider math for handling collisions and intersections


#include "BoxCollider.h"
#include "GameObject.h"
#include "Gizmos.h"

BoxCollider::~BoxCollider()
{
	delete m_boundingBox;
	m_boundingBox = nullptr;
}

auto BoxCollider::CollisionCheck(std::shared_ptr<Collider> a_other) -> bool
{
	// TODO(zac): figure out how to do collisions with other types of colliders??
	if (const std::shared_ptr<BoxCollider> otherCol = std::dynamic_pointer_cast<BoxCollider>(a_other))
	{
		return m_boundingBox->Intersects(otherCol->m_boundingBox);
	}
	return false;
}


void BoxCollider::Init()
{
	p_transform = m_owner->GetComponent<Transform>().get();
	//the warning visual studio gives here is a lie, honest. The pointer does not dangle. I hope.
	m_boundingBox = new AABB(p_transform->GetTranslation(), 1.0F);
	m_bRenderable = true;
}

void BoxCollider::Update(float /*deltaTime*/)
{
	m_boundingBox->SetOrigin(p_transform->GetTranslation());
}

void BoxCollider::Draw(unsigned int a_shader)
{
	Gizmos::addBox(m_boundingBox->GetCentre(), glm::vec3(1.0f), false);
}
