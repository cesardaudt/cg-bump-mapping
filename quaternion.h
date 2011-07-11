#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "vector3f.h"
#include "mathUtils.h"
#include <cmath>

class Quaternion {
	protected:
		float scalar;
		Vector3f vector;
	public:
		Quaternion(Vector3f base, float degrees) { scalar = cos(TO_RAD(degrees/2)); vector = base.normalized() * sin(TO_RAD(degrees/2)); }
		~Quaternion() { }

		Vector3f rotate(const Vector3f& v);
};

#endif
