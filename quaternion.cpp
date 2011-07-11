#include "quaternion.h"

Vector3f Quaternion::rotate(const Vector3f& v) {
	float x, y, z;
	x = (1 - 2*vector.getY()*vector.getY() - 2*vector.getZ()*vector.getZ())*v.getX() + (2*vector.getX()*vector.getY() - 2*vector.getZ()*scalar)*v.getY() + (2*vector.getX()*vector.getZ() + 2*vector.getY()*scalar)*v.getZ();
	y = (2*vector.getX()*vector.getY() + 2*vector.getZ()*scalar)*v.getX() + (1 - 2*vector.getX()*vector.getX() - 2*vector.getZ()*vector.getZ())*v.getY() + (2*vector.getY()*vector.getZ() - 2*vector.getX()*scalar)*v.getZ();
	z = (2*vector.getX()*vector.getZ() - 2*vector.getY()*scalar)*v.getX() + (2*vector.getY()*vector.getZ() + 2*vector.getX()*scalar)*v.getY() + (1 - 2*vector.getX()*vector.getX() - 2*vector.getY()*vector.getY())*v.getZ();

	return Vector3f(x, y, z);
}
