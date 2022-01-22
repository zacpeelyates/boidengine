
#include <glm.hpp>

#ifndef BOID_H
#define BOID_H

class Vec3;

class Boid
{
public:

	Boid(glm::mat4 a_inWorldMatrix, double a_inVel);

	double velocity;
	glm::mat4 WorldMatrix;

};

#endif // BOID_H
