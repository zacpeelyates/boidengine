// File: Octree.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Declarations for BSP Octree using existing AABB functionality

#ifndef OCTREE_H
#define OCTREE_H
#include <glm.hpp>
#include <vector>

class GameObject; //fwd declare
class AABB;

class Octree
{
public:
	Octree(AABB* a_bounds, Octree* a_parent = nullptr) : m_bounds(a_bounds), m_parent(a_parent), m_children(),m_objects(nullptr)
	{
		//create tree
		if (root == nullptr) root = this;
		for (int i = 0; i < 8; ++i)
		{
			m_children[i] = nullptr;
		}
	};
	~Octree() = default;
	//setup
	void Build(int a_depth);
	//statics
	static Octree* root;
	static void PartitionObjects(std::vector<GameObject*> &a_objects, Octree* startingNode);
	static Octree* Create(AABB* a_bounds, int a_depth);
	static void Draw(Octree* node);
	static void Draw() { Draw(root); }
	static Octree* GetContainingNode(glm::vec3 point, Octree* node = root);

	void Clear() const;
	bool Intersects(AABB* a_box) const;
	void Emplace(GameObject* a_object) const;
	bool isLeaf() const { return m_children[0] == nullptr; }
	bool Contains(GameObject* a_object) const;
	
	std::vector<GameObject*> GetObjects() const;
	std::vector<GameObject*>* GetObjects(GameObject* a_object) const;
	std::vector<GameObject*>* GetMemberObjectPointer() const { return m_objects; }

	
protected:
	AABB* m_bounds; //bounding box
	Octree* m_parent;
	Octree* m_children[8];
	std::vector<GameObject*>* m_objects; //gameobjects in the AABB

	
};
#endif // OCTREE_H
