// File: Transform.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/02/22
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Implements behaviours for transform component.


#include "Transform.h"
#include <gtx/orthonormalize.hpp>

auto Transform::GetScale() -> glm::vec3
{
	UpdateDecomposition();
	return m_decomposedMatrix.scale;
}

auto Transform::GetRotation() -> glm::quat
{
	UpdateDecomposition();
	return m_decomposedMatrix.rotation;
}

auto Transform::GetTranslation() -> glm::vec3
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

auto Transform::Orthonormalize() -> glm::mat4&
{
	glm::mat3 o = orthonormalize(glm::mat3(m_worldMatrix));
	for (int i = 0; i < 3; ++i)
	{
		SetRow(static_cast<MATRIX_ROW>(i), o[i]);
	}
	return m_worldMatrix;
}
