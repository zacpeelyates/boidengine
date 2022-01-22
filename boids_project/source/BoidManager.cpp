#include "BoidManager.h"
#include "Boid.h"
#include "Utilities.h"

#include <matrix.hpp>
#include <ext.hpp>

BoidManager* BoidManager::m_instance = nullptr;

BoidManager::BoidManager()
{
	m_boidCount = 100;
	m_maxBoidVelocity = 5;
	m_minBoidSeperation = 10;
}

BoidManager* BoidManager::CreateInstance()
{
	if (m_instance == nullptr) m_instance = new BoidManager();
	return m_instance;
}

BoidManager* BoidManager::GetInstance()
{
	return m_instance == nullptr ? CreateInstance() : m_instance;
}

void BoidManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

bool BoidManager::GenerateBoids()
{
	if(!boids.empty()) return false;
	for (int i = 0; i < m_boidCount; ++i)
	{	
		
	}
	return true;
}

bool BoidManager::UpdateBoids()
{
	for (Boid* b : boids)
	{
		std::vector<Boid*> nearbyBoids = GetVisableBoids(b);
	}

	return true;
}

std::vector<Boid*> BoidManager::GetVisableBoids(Boid* a_boid)
{
	return std::vector<Boid*>();
}

BoidManager::~BoidManager()
{
	for (Boid* b : boids)
	{
		delete b;
		b = nullptr;
	}
}
