#include "testing.h"
#include "../physics/boundingSphere.h"
#include "../physics/aabb.h"
#include "../physics/plane.h"
#include "../physics/physicsObject.h"
#include <iostream>
#include <cassert>

void Testing::runAllTests() {
	BoundingSphere::test();
	AABB::test();
	Plane::test();
	PhysicsObject::test();
}