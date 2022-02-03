#include "GameObject.h"



GameObject::GameObject(bool a_bIsRenderable)
{
	m_bRenderable = a_bIsRenderable;
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

void GameObject::Draw(Shader* a_pShader)
{
}
