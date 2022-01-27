
#include "BoxCollider.h"

BoxCollider::~BoxCollider(){}

bool BoxCollider::CollisionCheck(std::shared_ptr<Collider> a_other)
{
	//TODO: figure out how to do collisions with other types of colliders??
	std::shared_ptr<BoxCollider> other = std::dynamic_pointer_cast<BoxCollider>(a_other);
	if (other)
	{
		return m_boundingBox.Intersects(other->m_boundingBox);
	}
	return false;
}

