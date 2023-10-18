// File: BoxCollider.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/04/11
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Declarations for AABB-based colliders


#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H
#include "Component.h"
#include "Collider.h"
#include "AABB.h"

class BoxCollider final : public Collider
{
public: 
	BoxCollider(GameObject* a_owner) : Collider(a_owner) {};
	~BoxCollider() override;

	bool CollisionCheck(std::shared_ptr<Collider> a_other) override;

	void Init() override;
	void Update(float deltaTime) override;
	void Draw(unsigned int a_shader) override;
	AABB* m_boundingBox = nullptr;
	
};
#endif // BOXCOLLIDER_H
