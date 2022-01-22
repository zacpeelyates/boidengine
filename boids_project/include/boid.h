
#ifndef BOID_H
#define BOID_H

struct Position
{
		Position(const double d_x, const double d_y, const double d_z)
		{
			x = d_x;
			y = d_y;
			z = d_z;
		}
		double x;
		double y;
		double z;
};

struct View
{
	View(const double d_angle, const double d_radius)
	{
		angle = d_angle;
		radius = d_radius;
	}
	double angle;
	double radius;
};

struct Weights
{
	Weights(const double d_c, const double d_s, const double d_a, const double d_f, const double d_o)
	{
		cohesion = d_c;
		separation = d_s;
		alignment = d_a;
		flee = d_f;
		obstacleAvoid = d_o;
	}

	double cohesion;
	double separation;
	double alignment;
	double flee;
	double obstacleAvoid;
};

class Boid
{
public:

	Position pos;
	View view;
	double velocity;
	double direction;
	double size;

	
		
};

#endif // BOID_H
