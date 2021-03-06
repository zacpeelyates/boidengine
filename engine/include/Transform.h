
#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "glm.hpp"
#include "gtx/matrix_decompose.hpp"
#include  "Component.h"

enum MATRIX_ROW
{
	RIGHT_VEC,
	UP_VEC,
	FWD_VEC,
	POS_VEC
};



class Transform : public Component
{
public:

	Transform(GameObject* a_owner) : Component(a_owner), m_worldMatrix(glm::mat4(1.0f)), m_decomposedMatrix() { UpdateDecomposition(); };

	inline glm::mat4 GetTransformMatrix() const { return m_worldMatrix; }
	inline void SetTransformMatrix(glm::mat4 a_matrix) { m_worldMatrix = a_matrix; }

	inline void Translate(glm::vec3 a_translation) { m_worldMatrix = glm::translate(m_worldMatrix, a_translation); }
	inline void Rotate(glm::f32 a_degrees, glm::vec3 a_normalizedAxis)  { m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(a_degrees), a_normalizedAxis); }
	inline void Scale(glm::vec3 a_scale)  {m_worldMatrix = glm::scale(m_worldMatrix, a_scale); }

	inline glm::vec3 GetRow(MATRIX_ROW a_row) { return m_worldMatrix[a_row]; }
	inline void SetRow(MATRIX_ROW a_row, glm::vec3 a_vec) { m_worldMatrix[a_row] = glm::vec4(a_vec, a_row == POS_VEC ? 1.0f : 0.0f); }

	//decompositions
	glm::vec3 GetScale();
	glm::quat GetRotation();
	glm::vec3 GetTranslation();

	void SetScale(glm::vec3 a_scale);
	void SetRotation(glm::quat a_rotation);
	void SetTranslation(glm::vec3 a_translation);

	//get matrix ref
	inline glm::mat4& GetMatrix() { return m_worldMatrix; }

private:
	glm::mat4 m_worldMatrix;

	struct MATRIX_DECOMPOSITION
	{
		//components
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		//not used in recomposition, needed for glm::decompose method
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::mat4 RecomposeMatrix()
		{
			glm::mat4 result = glm::mat4(1);

			result = glm::translate(result, translation);
			result = glm::scale(result, scale);
			result *= glm::mat4_cast(rotation);
			return result;
		}
	};

	MATRIX_DECOMPOSITION m_decomposedMatrix;
	inline void UpdateDecomposition() { glm::decompose(m_worldMatrix, m_decomposedMatrix.scale, m_decomposedMatrix.rotation, m_decomposedMatrix.translation, m_decomposedMatrix.skew, m_decomposedMatrix.perspective); }
public:
	void Update(float a_deltaTime) override {};
	void Init() override {};
	void Draw(unsigned a_shader) override {};
};
#endif // TRANSFORM_H
