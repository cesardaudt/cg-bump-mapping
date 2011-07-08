#include "vector3f.h"
using namespace std;

Vector3f Vector3f::crossProduct(const Vector3f& v) const {
	float nx = (y * v.z) - (z * v.y);
	float ny = (z * v.x) - (x * v.z);
	float nz = (x * v.y) - (y * v.x);

	return Vector3f(nx, ny, nz);
}

void Vector3f::normalizeMe() {
	float norm = getNorm();
	x /= norm;
	y /= norm;
	z /= norm;
}

Vector3f Vector3f::normalized() const {
	float norm = getNorm();
	return Vector3f(x/norm, y/norm, z/norm);
}

Vector3f& Vector3f::operator=(const Vector3f& v) {
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

Vector3f Vector3f::operator+(const Vector3f& v) const {
	return Vector3f(x+v.x, y+v.y, z+v.z);
}

Vector3f Vector3f::operator-(const Vector3f& v) const {
	return Vector3f(x-v.x, y-v.y, z-v.z);
}

Vector3f Vector3f::operator*(const float f) const {
	return Vector3f(x*f, y*f, z*f);
}

Vector3f Vector3f::operator/(const float f) const {
	return Vector3f(x/f, y/f, z/f);
}

void Vector3f::print() const {
	cout << "<" << x << ", " << y << ", " << z << ">" << endl;
}
