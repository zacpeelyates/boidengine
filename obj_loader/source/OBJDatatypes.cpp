// File: OBJDatatypes.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/02/16
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Function implementations for various OBJ data classes


#include "OBJDatatypes.h"



//OBJmodel

bool OBJModel::AddGroup(OBJGroup* ao_groupIn)
{
	if (m_groupMap.find(ao_groupIn->name) == m_groupMap.end()) 
	{
		m_groupMap.emplace(ao_groupIn->name, ao_groupIn);
		return true;
	}
	return false;

}

unsigned int OBJModel::GetMeshCount() const
{
	return m_meshes.size();
}


OBJMesh* OBJModel::GetMesh(unsigned int a_uiIndex) const
{
	const unsigned int meshCount = m_meshes.size();
	if (meshCount > 0 && a_uiIndex < meshCount)
	{
		return m_meshes[a_uiIndex];
	}
	return nullptr;
}

OBJMaterial* OBJModel::GetMaterial(std::string a_name)
{
	const auto mat = m_matMap.find(a_name);
	if (mat == m_matMap.end()) return nullptr;
	return mat->second;
}

OBJMaterial* OBJModel::GetMaterial(unsigned int index)
{
	if (index > m_matMap.size()) return nullptr;
	auto iter = m_matMap.begin();
	std::advance(iter, index);
	return iter->second;

}

unsigned int OBJModel::GetMaterialCount() const
{
	return m_matMap.size();
}

glm::mat4 OBJModel::GetWorldMatrix() const
{
	return m_worldMatrix;
}

void OBJModel::SetWorldMatrix(glm::mat4 a_inMatrix)
{
	m_worldMatrix = a_inMatrix;
}

bool OBJModel::AddMaterial(OBJMaterial* a_poInMaterial)
{
	if (m_matMap.find(a_poInMaterial->name) == m_matMap.end())
	{
		m_matMap.emplace(a_poInMaterial->name, a_poInMaterial);
		return true;
	}
	return false;
}

void OBJModel::AddMesh(OBJMesh* a_InMesh)
{
	m_meshes.push_back(a_InMesh);
}

//OBJVertex

glm::vec3 OBJVertex::GetPosition() const
{
	return m_pos;
}

glm::vec3 OBJVertex::GetNormal() const
{
	return m_normal;
}

glm::vec2 OBJVertex::GetTextureCoords() const
{
	return m_uvCoord;
}

void OBJVertex::SetPosition(glm::vec3 a_v3New)
{
	m_pos = a_v3New;
}

void OBJVertex::TranslatePosition(glm::vec3 a_v3Delta)
{
	m_pos += a_v3Delta;
}

void OBJVertex::SetNormal(glm::vec3 a_v3New)
{
	m_normal = a_v3New;
}

void OBJVertex::SetTextureCoords(glm::vec2 a_v2New)
{
	m_uvCoord = a_v2New;
}


bool OBJVertex::operator==(const OBJVertex& a_oOther) const
{
	return memcmp(this,&a_oOther,sizeof(OBJVertex)) == 0;
}

bool OBJVertex::operator<(const OBJVertex& a_oOther) const
{
	return memcmp(this, &a_oOther, sizeof(OBJVertex)) < 0;
}

//OBJMesh

glm::vec3 OBJMesh::calculateFaceNormal(const unsigned int& a_indexA, const unsigned int& a_indexB, const unsigned int& a_indexC) const
{
	const glm::vec3 a = m_verts[a_indexA].GetPosition();
	const glm::vec3 b = m_verts[a_indexB].GetPosition();
	const glm::vec3 c = m_verts[a_indexC].GetPosition();

	return cross(normalize(b - c), normalize(c - a));
}
void OBJMesh::CalculateUnassignedFaceNormals()
{
	for (unsigned int i = 0; i < m_indicies.size(); i += 3)
	{
		const glm::vec3 n = calculateFaceNormal(i, i + 1, i + 2);
		m_verts[i].SetNormal(n);
		m_verts[i+1].SetNormal(n);
		m_verts[i+2].SetNormal(n);
	}
}

//OBJMaterial

glm::vec3 OBJMaterial::GetAmbience()
{
	return ambience;
}

glm::vec3 OBJMaterial::GetDiffuse()
{
	return diffuse;
}

glm::vec3 OBJMaterial::GetSpecular()
{
	return specular;
}

glm::vec3 OBJMaterial::GetEmissive()
{
	return emissive;
}

float OBJMaterial::GetTransparency()
{
	return transparency;
}

void OBJMaterial::SetAmbience(glm::vec3 a_inv3)
{
	ambience = a_inv3;
}

void OBJMaterial::SetDiffuse(glm::vec3 a_inv3)
{
	diffuse = a_inv3;
}

void OBJMaterial::SetSpecular(glm::vec3 a_inv3)
{
	specular = a_inv3;
}

void OBJMaterial::SetEmissive(glm::vec3 a_inv3)
{
	emissive = a_inv3;
}

void OBJMaterial::SetTransparency(float a_fIn)
{
	transparency = a_fIn;
}

float OBJMaterial::GetDensity()
{
	return density;
}

float OBJMaterial::GetSpecularExponent()
{
	return specularExponent;
}

float OBJMaterial::GetDissolve()
{
	return dissolve;
}

void OBJMaterial::SetDensity(float a_fIn)
{
	density = a_fIn;
}

void OBJMaterial::SetDissolve(float a_fIn)
{
	dissolve = a_fIn;
}

void OBJMaterial::SetSpecularExponent(float a_fIn)
{
	specularExponent = a_fIn;
}

uint8_t OBJMaterial::GetIlluminationModel() const
{
	return illumModel;
}

void OBJMaterial::SetIlluminationModel(uint8_t a_inUint)
{
	illumModel = a_inUint;
}
