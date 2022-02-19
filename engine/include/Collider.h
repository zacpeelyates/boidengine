#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include "Transform.h"
#include <memory>

class Collider : public Component
{
public:
	virtual ~Collider() {};
	virtual bool CollisionCheck(std::shared_ptr<Collider> other) = 0;
	Collider(GameObject* a_owner, bool a_trigger = false) : Component(a_owner), bIsTrigger(a_trigger), p_transform(nullptr) {};

	virtual void Init() override = 0;
	virtual void Start() override = 0;
	virtual void Update(float a_deltaTime) override = 0;
	virtual void Draw(unsigned a_shader) override = 0;

protected:
	bool bIsTrigger;
	Transform* p_transform;
};
#endif //COLLDER_H
