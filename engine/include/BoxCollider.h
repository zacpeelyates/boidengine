
#include "Component.h"
#include "Collider.h"
#include "Transform.h"
#include "AABB.h"

class BoxCollider final : public Collider
{
public: 
	BoxCollider(GameObject* a_owner) : Collider(a_owner) {};
	~BoxCollider();

	bool CollisionCheck(std::shared_ptr<Collider> a_other) override;

protected:
	AABB m_boundingBox;
};