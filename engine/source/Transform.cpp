
#include "Transform.h"

glm::vec3 Transform::GetScale()
{
	UpdateDecomposition();
	return m_decomposedMatrix.scale;
}

glm::quat Transform::GetRotation()
{
	UpdateDecomposition();
	return m_decomposedMatrix.rotation;
}

glm::vec3 Transform::GetTranslation()
{
	UpdateDecomposition();
	return m_decomposedMatrix.translation;
}

void Transform::SetScale(glm::vec3 a_scale)
{
	m_decomposedMatrix.scale = a_scale;
	m_worldMatrix = m_decomposedMatrix.RecomposeMatrix();

}

void Transform::SetRotation(glm::quat a_rotation)
{
	m_decomposedMatrix.rotation = a_rotation;
	m_worldMatrix = m_decomposedMatrix.RecomposeMatrix();
}

void Transform::SetTranslation(glm::vec3 a_translation)
{
	m_decomposedMatrix.translation = a_translation;
	m_worldMatrix = m_decomposedMatrix.RecomposeMatrix();
}


