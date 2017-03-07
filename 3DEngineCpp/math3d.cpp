#include "math3d.h"

Vector3f Vector3f::rotate(const Quaternion& rotation) const
{
	Quaternion conjugateQ = rotation.conjugate();
	Quaternion w = rotation * (*this) * conjugateQ;

	Vector3f ret(w.getX(), w.getY(), w.getZ());

	return ret;
}
