
#include "Transform.h"
#include "gtx/matrix_decompose.hpp"

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

glm::vec3 Transform::GetSkew()
{
	UpdateDecomposition();
	return m_decomposedMatrix.skew;
}

glm::vec4 Transform::GetPerspective()
{
	UpdateDecomposition();
	return m_decomposedMatrix.perspective;
}

void Transform::SetScale(glm::vec3 a_scale)
{
	m_decomposedMatrix.scale = a_scale;
}

void Transform::SetRotation(glm::quat a_rotation)
{
	m_decomposedMatrix.rotation = a_rotation;
}

void Transform::SetTranslation(glm::vec3 a_translation)
{
	m_decomposedMatrix.translation = a_translation;
}

void Transform::SetSkew(glm::vec3 a_skew)
{
	m_decomposedMatrix.skew = a_skew;
}

void Transform::SetPerspective(glm::vec4 a_perspective)
{
	m_decomposedMatrix.perspective = a_perspective;
}

