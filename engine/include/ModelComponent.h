#ifndef MODEL
#define MODEL_H

#include "Component.h"
class Model;

class ModelComponent : public Component
{
public:
	ModelComponent(GameObject* a_owner) : Component(a_owner) {}
	~ModelComponent() {};

	 virtual void Draw(Shader* a_pShader);
	 inline void SetModel(Model* a_pModel) {m_modelData = a_pModel};
private:
	Model* m_modelData;

};
#endif //!Model