#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/glut.h>
#include "vector3f.h"
#include "quaternion.h"
#include <cstdio>

class Camera {
	protected:
		Vector3f up;
		Vector3f lookAt;
		Vector3f third;

		Vector3f eye; // Position
	public:
		Camera() { eye = Vector3f(0,0,0); lookAt = Vector3f(1,0,0); up = Vector3f(0,1,0); third = lookAt.crossProduct(up); };
		Camera(Vector3f eye, Vector3f lookAt, Vector3f up);
		~Camera() { };

		inline void setPosition(const Vector3f& pos) { eye = pos; }
		inline void setTarget(const Vector3f& lookAt, const Vector3f& up) {
			this->lookAt = lookAt;//.normalized();
			this->third = this->lookAt.normalized().crossProduct(up.normalized());
			this->up = this->third.crossProduct(this->lookAt.normalized());
		}

		inline Vector3f getPosition() const { return eye; }
		inline Vector3f getUp() const { return up; }
		inline Vector3f getLookAt() const { return lookAt; }
		inline Vector3f getThird() const { return third; }

		void setPerspective(const double fovy, const double aspect, const double zNear, const double zFar);
		void setFrustum(const double fovx, const double fovy, const double zNear, const double zFar);

		void translate(const Vector3f& v, const bool lockLookat=false, const bool cameraCoordinates = true);
		void rotateBase(const Vector3f& v, const float degrees);

		void look();
};

#endif
