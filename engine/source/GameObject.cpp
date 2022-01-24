#include "GameObject.h"

void GameObject::Init()
{
	for(const auto& c : m_components)
	{
		c->Init();
	}
}

void GameObject::Start()
{
	for (const auto& c : m_components)
	{
		c->Start();
	}
}

void GameObject::Update(float a_deltaTime)
{
	for (const auto& c : m_components)
	{
		c->Update();
	}
}
