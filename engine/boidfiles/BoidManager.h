#include <vector>

struct BoidWeights
{
	BoidWeights(const double d_c, const double d_s, const double d_a, const double d_o)
	{
		cohesion = d_c;
		separation = d_s;
		alignment = d_a;
		obstacleAvoid = d_o;
	}

	double cohesion;
	double separation;
	double alignment;
	double obstacleAvoid;
};

struct BoidViewInfo
{
	BoidViewInfo(const double d_angle, const double d_radius)
	{
		angle = d_angle;
		radius = d_radius;
	}
	double angle;
	double radius;
};

class Boid; 

class BoidManager
{
public: 

	std::vector<Boid*> boids;
	static BoidWeights boidWeights;
	static BoidViewInfo boidViewInfo;

	static BoidManager* CreateInstance();
	static BoidManager* GetInstance();
	void DestroyInstance();

	bool GenerateBoids();

	bool UpdateBoids();

	std::vector<Boid*> GetVisableBoids(Boid* a_boid);

private:

	BoidManager();
	~BoidManager();
	static BoidManager* m_instance;
	int m_boidCount;
	int m_maxBoidVelocity;
	int m_minBoidSeperation;

};