#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"
#include <memory>

class Collider : public Component
{
public: 
	Collider(GameObject* a_owner,bool a_trigger = false) : Component(a_owner), bIsTrigger(a_trigger) {};
	~Collider() {};
	virtual bool CollisionCheck(std::shared_ptr<Collider> other) = 0;

private:
	bool bIsTrigger;
};

#endif //COLLDER_HL