#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <memory>
#include <type_traits>
#include <vector>

#include "Component.h"

class GameObject
{
public:

	GameObject() = default;

	template<typename T> std::shared_ptr<T> AddComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "Object is not component!"); //ensure we are dealing with a component object
		for(auto& c : m_components) //check if we already have a component of this type
		{
			if (auto s = std::dynamic_pointer_cast<T>(c)) return s;
		}
		//component is of new type
		auto component = std::make_shared<T>(this);
		m_components.push_back(component);
		return component;
	}

	template<typename T> std::shared_ptr<T> GetComponent()
	{
		for (auto c : m_components)
		{
			if (auto s = std::dynamic_pointer_cast<T>(c)) return s;
		}
		return nullptr;
	}


	void Init();
	void Start();
	void Update(float a_deltaTime);

protected:
	std::vector<std::shared_ptr<Component>> m_components;
};

#endif // GAMEOBJECT_H
