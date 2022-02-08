#include "GameObject.h"

std::vector<GameObject*> GameObject::s_GameObjects = std::vector<GameObject*>();

GameObject::GameObject(bool a_bIsRenderable)
{
	m_bRenderable = a_bIsRenderable;
	s_GameObjects.push_back(this);
}

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
		c->Update(a_deltaTime);
	}
}

void GameObject::Draw(unsigned int a_shader)
{
	if (m_bRenderable) return;
	for (const auto& c : m_components)
	{
		c->Draw(a_shader);
	}
}
