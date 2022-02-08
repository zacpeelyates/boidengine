#ifndef MODEL
#define MODEL_H

#include "Component.h"
class OBJModel;

class ModelComponent : public Component
{
public:
	ModelComponent(GameObject* a_owner, OBJModel* a_data = nullptr) : Component(a_owner), m_modelData(a_data) {}
	~ModelComponent() {};

	 virtual void Draw(unsigned int a_shader);
	 virtual void Update(float a_deltaTime);
	 inline void SetModel(OBJModel* a_pModel) { m_modelData = a_pModel; };
	 OBJModel* GetModel() { return m_modelData; };
private:
	OBJModel* m_modelData;

};
#endif //!Model