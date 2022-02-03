#include "Boid.h"
#include "Transform.h"
#include "BoxCollider.h"

Boid::Boid()
{
	AddComponent<Transform>();
	AddComponent<BoxCollider>();
}

//boid stuff here