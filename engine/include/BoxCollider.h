
#include "Component.h"
#include "Collider.h"
#include "AABB.h"

class BoxCollider final : public Collider
{
public: 
	BoxCollider(GameObject* a_owner) : Collider(a_owner) {};
	~BoxCollider();

	bool CollisionCheck(std::shared_ptr<Collider> a_other) override;

	void Init() override;
	void Start() override;
	void Update(float deltaTime) override;
	void Draw(unsigned int a_shader) override;

protected:
	AABB* m_boundingBox;
};