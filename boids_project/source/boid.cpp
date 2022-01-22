#include "Boid.h"
#include "Utilities.h"

Boid::Boid(glm::mat4 a_inWorldMatrix, double a_inVel)
{
	WorldMatrix = a_inWorldMatrix;
	velocity = a_inVel;
}
