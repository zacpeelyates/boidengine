
#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "glm.hpp"
#include "ext.hpp"
#include  "Component.h"
class Transform : Component
{
public:

	void Init() override { m_worldMatrix = glm::mat4(1); };
	void Start() override = 0;
	void Update() override = 0;

	glm::mat4 GetTransformMatrix() const { return m_worldMatrix; }
	void SetTransformMatrix(glm::mat4 a_matrix) { m_worldMatrix = a_matrix; }

	void Translate(glm::vec3 a_translation) const { glm::translate(m_worldMatrix, a_translation); }
	void Rotate(glm::f32 a_degrees, glm::vec3 a_normalizedAxis) const { glm::rotate(m_worldMatrix, glm::radians(a_degrees), a_normalizedAxis); }
	void Scale(glm::vec3 a_scale) const { glm::scale(m_worldMatrix, a_scale); }

private:
	glm::mat4 m_worldMatrix;
};
#endif // TRANSFORM_H
