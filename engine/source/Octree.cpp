// File: Octree.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Implementation of OCTREE space partitioning

#include "Octree.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include <Gizmos.h>

Octree* Octree::root = nullptr;

void Octree::Build(int a_depth)
{
	if (a_depth == 0)
	{
		m_objects = new std::vector<GameObject*>();
		for (auto& i : m_children)
		{
			i = nullptr;
		}
		return;
	}

	glm::vec3 corners[8];
	m_bounds->GetPoints(corners);
	glm::vec3 centre = m_bounds->GetCentre();
	for (int i = 0; i < 8; ++i)
	{
		m_children[i] = new Octree(new AABB(centre, corners[i]), this);
		Gizmos::addBox(m_children[i]->m_bounds->GetCentre(), m_children[i]->m_bounds->GetMax(), false);
		m_children[i]->Build(a_depth - 1);
	}
}

void Octree::PartitionObjects(std::vector<GameObject*>& a_objects, Octree* a_octree)
{
	for (GameObject* g : a_objects)
	{
		a_octree->Emplace(g);
	}
}

auto Octree::Intersects(AABB* a_box) const -> bool
{
	return m_bounds->Intersects(a_box);
}

void Octree::Emplace(GameObject* a_object) const
{
	if (isLeaf())
	{
		m_objects->push_back(a_object);
		return;
	}

	AABB* col = a_object->GetComponent<BoxCollider>()->m_boundingBox;
	if (!Intersects(col))
	{
		return;
	}
	for (auto i : m_children)
	{
		if (!i->Intersects(col))
		{
			continue;
		}
		i->Emplace(a_object);
	}
}

auto Octree::Contains(GameObject* a_gameObject) const -> bool
{
	if (isLeaf())
	{
		const auto a = *m_objects;
		for (int i = 0; i < m_objects->size(); ++i)
		{
			if (a[i] == a_gameObject)
			{
				return true;
			}
		}
		return false;
	}

	for (auto i : m_children)
	{
		if (i->Contains(a_gameObject))
		{
			return true;
		}
	}
	return false;
}

auto Octree::GetContainingNode(glm::vec3 a_point, Octree* a_node) -> Octree*
{
	if (a_node->isLeaf())
	{
		return a_node;
	}
	for (auto& i : a_node->m_children)
	{
		if (i->m_bounds->Contains(a_point))
		{
			return GetContainingNode(a_point, i);
		}
	}
	return nullptr;
}

auto Octree::GetObjects() const -> std::vector<GameObject*>
{
	if (isLeaf())
	{
		return *m_objects;
	}

	std::vector<GameObject*> result;
	for (auto i : m_children)
	{
		std::vector<GameObject*> next = i->GetObjects();
		result.insert(result.end(), next.begin(), next.end());
	}
	return result;
}

auto Octree::GetObjects(GameObject* a_object) const -> std::vector<GameObject*>*
{
	std::vector<GameObject*>* result = nullptr;
	if (!Contains(a_object))
	{
		return result;
	}
	if (isLeaf())
	{
		return m_objects;
	}
	for (auto i : m_children)
	{
		if (i->Contains(a_object))
		{
			result = i->GetObjects(a_object);
			break;
		}
	}
	return result;
}

auto Octree::Create(AABB* a_bounds, int depth) -> Octree*
{
	auto* const root = new Octree(a_bounds);
	root->Build(depth);
	return root;
}

void Octree::Draw(Octree* node)
{
	if (node->isLeaf())
	{
		Gizmos::addBox(node->m_bounds->GetCentre(), node->m_bounds->GetDimensions(), false);
		return;
	}

	for (auto& i : node->m_children)
	{
		Draw(i);
	}
}

void Octree::Clear() const
{
	if (isLeaf())
	{
		m_objects->clear();
		return;
	}
	for (auto i : m_children)
	{
		i->Clear();
	}
}
