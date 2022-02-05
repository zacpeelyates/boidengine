#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <memory>
#include <type_traits>
#include <vector>

#include "Component.h"

class Shader;

class GameObject
{
public:

	GameObject() = default;
	GameObject(bool a_bIsRenderable);

	template<typename T> std::shared_ptr<T> AddComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "Object is not a component!"); //ensure we are dealing with a component object
		for(std::shared_ptr<Component>& c : m_components) //check if we already have a component of this type
		{
			if (std::shared_ptr<Component> s = std::dynamic_pointer_cast<T>(c)) return s;
		}
		//component is of new type
		auto component = std::make_shared<T>(this);
		m_components.push_back(component);
		return component;
	}

	template<typename T> std::shared_ptr<T> GetComponent()
	{
		for (std::shared_ptr<Component> c : m_components)
		{
			if (std::shared_ptr<T> s = std::dynamic_pointer_cast<T>(c)) return s;
		}
		return nullptr;
	}

	virtual void Init();
	virtual void Start();
	virtual void Update(float a_deltaTime);
	virtual void Draw(Shader* a_pShader);

protected:
	std::vector<std::shared_ptr<Component>> m_components;
	bool m_bRenderable;
};

#endif // GAMEOBJECT_H
