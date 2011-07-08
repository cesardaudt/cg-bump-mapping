#ifndef _VECTOR3F_H_
#define _VECTOR3F_H_

#include <cmath>
#include <iostream>

class Vector3f {
	protected:
		float x;
		float y;
		float z;
	public:
		Vector3f() {
			x = 0;
			y = 0;
			z = 0;
		};

		Vector3f(const float x, const float y, const float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		~Vector3f() { };

		inline const float getX() const {
			return x;
		}

		inline void setX(float x) {
			this->x = x;
		}

		inline const float getY() const {
			return y;
		}

		inline void setY(float y) {
			this->y = y;
		}

		inline const float getZ() const {
			return z;
		}

		inline void setZ(float z) {
			this->z = z;
		}

		inline const float dotProduct(const Vector3f& v) const {
			return ((x*v.x) + (y*v.y) + (z*v.z));
		}

		Vector3f crossProduct(const Vector3f& v) const;

		inline float getNorm() const {
			return sqrt((x*x) + (y*y) + (z*z));
		}

		void normalizeMe();
		Vector3f normalized() const;

		Vector3f& operator=(const Vector3f& v);
		Vector3f operator+(const Vector3f& v) const;
		Vector3f operator-(const Vector3f& v) const;
		Vector3f operator*(const float f) const;
		Vector3f operator/(const float f) const;

		void print() const;
};

#endif
