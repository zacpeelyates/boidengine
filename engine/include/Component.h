
#ifndef COMPONENT_H
#define COMPONENT_H
class GameObject;
class Component
{
public:

	virtual ~Component() = default;
	Component(GameObject* a_owner, bool a_render = false) : m_owner(a_owner), m_bIsRenderable(a_render){}

	virtual void Init() = 0;
	virtual void Start() = 0;
	virtual void Update(float a_deltaTime) = 0;
	virtual void Draw(unsigned int a_shader) = 0;

protected:
	GameObject* m_owner;
	inline GameObject* GetOwner() const { return m_owner; };
	bool m_bIsRenderable; 

};
#endif // COMPONENT_H
