// File: Component.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Declarations for Components, behaviours which can be attached to gameobjects 


#ifndef COMPONENT_H
#define COMPONENT_H
class GameObject;
class Component
{
public:

	virtual ~Component() = default;
	Component(GameObject* a_owner, bool a_render = false) : m_bRenderable(a_render), m_owner(a_owner){}

	virtual void Init() = 0;
	virtual void Update(float a_deltaTime) = 0;
	virtual void Draw(unsigned int a_shader) = 0;
	bool m_bRenderable;

protected:
	GameObject* m_owner;
	GameObject* GetOwner() const { return m_owner; }

};
#endif // COMPONENT_H
