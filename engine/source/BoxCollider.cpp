
#include "BoxCollider.h"

BoxCollider::~BoxCollider(){}

bool BoxCollider::CollisionCheck(std::shared_ptr<Collider> a_other)
{
	//TODO: figure out how to do collisions with other types of colliders??
	if (std::shared_ptr<BoxCollider> otherCol = std::dynamic_pointer_cast<BoxCollider>(a_other))
	{
		return m_boundingBox.Intersects(otherCol->m_boundingBox);
	}
	return false;
}


