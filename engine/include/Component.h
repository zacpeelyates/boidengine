
#ifndef COMPONENT_H
#define COMPONENT_H
class GameObject;
class Component : Entity
{
public:

	virtual ~Component() = default;
	Component(GameObject* a_owner) : m_owner(a_owner){}

	virtual void Init() = 0;
	virtual void Start() = 0;
	virtual void Update(float a_deltaTime) = 0;
	virtual void Draw(Shader* a_pShader) = 0;

protected:
	GameObject* m_owner;
	inline GameObject* GetOwner() {return m_owner};

};
#endif // COMPONENT_H
