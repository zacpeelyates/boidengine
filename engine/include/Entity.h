#ifndef ENTITY_H
#define ENTITY_H

class Shader; 

class Entity
{

public:

	virtual void Init() = 0;
	virtual void Start() = 0;
	virtual void Update(float a_deltaTime) = 0;
	virtual void Draw(Shader* a_pShader) = 0;


};
#endif // !ENTITY_H
