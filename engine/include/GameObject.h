#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <memory>
#include <type_traits>
#include <vector>

class Component;

class GameObject
{
public:
	virtual ~GameObject() = default;

	GameObject(bool a_bIsRenderable = true);

	template<typename T> std::shared_ptr<T> AddComponent()
	{
		if (!std::is_base_of<Component, T>()) return nullptr; //early out 
		for(std::shared_ptr<Component>& c : m_components) //check if we already have a component of this type
		{
			std::shared_ptr<T> s = std::dynamic_pointer_cast<T>(c);
			if (s) return s;
		}
		//component is of new type
		auto component = std::make_shared<T>(this);
		m_components.push_back(component);
		return component;
	}

	template<typename T> std::shared_ptr<T> GetComponent()
	{
		if (!std::is_base_of<Component, T>()) return nullptr; //early out 
		for (std::shared_ptr<Component> c : m_components)
		{
			if (std::shared_ptr<T> s = std::dynamic_pointer_cast<T>(c)) return s;
		}
		return nullptr;
	}

	virtual void Init();
	virtual void Update(float a_deltaTime);
	virtual void Draw(unsigned int a_shader);
	static std::vector<GameObject*> s_GameObjects;

	template<typename T> static std::vector<T*> GetObjectsOfType()
	{
		if (!std::is_base_of<GameObject, T>()) return nullptr; //early out 
		std::vector<T*> result;
		for (GameObject* g : s_GameObjects)
		{
			if (T* t = std::dynamic_pointer_cast<T>(g)) result.push_back(t);
		}
		return result;
	}

protected:
	std::vector<std::shared_ptr<Component>> m_components;
	bool m_bRenderable{};
};

#endif // GAMEOBJECT_H
