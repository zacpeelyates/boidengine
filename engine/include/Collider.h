// File: Collider.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/02/22
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Declarations for generic colliders (used by boxcolliders and could in future be extended to other colliders)

#ifndef COLLIDER_H
#define COLLIDER_H

#include "Transform.h"
#include <memory>

class Collider : public Component
{
public:
	~Collider() override = default;
	virtual bool CollisionCheck(std::shared_ptr<Collider> other) = 0;
	Collider(GameObject* a_owner, bool a_trigger = false) : Component(a_owner), bIsTrigger(a_trigger), p_transform(nullptr) {}

	void Init() override = 0;
	void Update(float a_deltaTime) override = 0;
	void Draw(unsigned a_shader) override = 0;

protected:
	bool bIsTrigger;
	Transform* p_transform;
};
#endif //COLLDER_H
