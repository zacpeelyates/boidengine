
#include "BoxCollider.h"
#include "GameObject.h"

BoxCollider::~BoxCollider(){}

bool BoxCollider::CollisionCheck(std::shared_ptr<Collider> a_other)
{
	//TODO: figure out how to do collisions with other types of colliders??
	if (std::shared_ptr<BoxCollider> otherCol = std::dynamic_pointer_cast<BoxCollider>(a_other))
	{
		return m_boundingBox->Intersects(otherCol->m_boundingBox);
	}
	return false;
}

void BoxCollider::Init()
{
	p_transform = m_owner->GetComponent<Transform>().get(); //the warning visual studio gives here is a lie, honest. The pointer does not dangle. I hope.
	m_boundingBox = new AABB(p_transform->GetTranslation(), 0.5f);
}

void BoxCollider::Update(float deltaTime)
{
	m_boundingBox->SetOrigin(p_transform->GetTranslation());
}

void BoxCollider::Draw(unsigned int a_shader)
{

}


