// File: GameObject.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Generic GameObject class, handles Initilization, Updating & Drawing of itself and all components.

#include "GameObject.h"
#include <Transform.h>

std::vector<GameObject*> GameObject::s_GameObjects = std::vector<GameObject*>();

GameObject::GameObject()
{
	s_GameObjects.push_back(this);
}

void GameObject::Init()
{
	for (const std::shared_ptr<Component>& c : m_components)
	{
		c->Init();
	}
}

void GameObject::Update(float a_deltaTime)
{
	for (const std::shared_ptr<Component>& c : m_components)
	{
		c->Update(a_deltaTime);
	}
}

void GameObject::Draw(unsigned int a_shader)
{
	for (const std::shared_ptr<Component>& c : m_components)
	{
		if (c->m_bRenderable)
		{
			c->Draw(a_shader);
		}
	}
}
