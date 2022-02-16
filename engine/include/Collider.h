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

	void Init() override;
	void Start() override;
	void Update(float a_deltaTime) override;
	void Draw(unsigned a_shader) override;

private:
	bool bIsTrigger;
};

inline void Collider::Init()
{
}

inline void Collider::Start()
{
}

inline void Collider::Update(float a_deltaTime)
{
}

inline void Collider::Draw(unsigned a_shader)
{
}
#endif //COLLDER_HL
