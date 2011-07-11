#include "camera.h"

Camera::Camera(Vector3f eye, Vector3f lookAt, Vector3f up) {
	this->eye = eye;
	this->setTarget(lookAt, up);
}

void Camera::setPerspective(const double fovy, const double aspect, const double zNear, const double zFar) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, zNear, zFar);
}

void Camera::setFrustum(const double fovx, const double fovy, const double zNear, const double zFar) {
	float w, h;
	h = zNear * tan(TO_RAD(fovy/2));
	w = zNear * tan(TO_RAD(fovx/2));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-w/2, w/2, -h/2, h/2, zNear, zFar);
}

void Camera::translate(const Vector3f& v, const bool lockLookat, const bool cameraCoordinates) {
	if(cameraCoordinates) {
		Vector3f nv = v.normalized();

		Vector3f tz = lookAt.normalized() * v.getZ();
		Vector3f ty = up * v.getY();
		Vector3f tx = third * v.getX();
		Vector3f translation = tx + ty + tz;

		Vector3f target = eye + lookAt;
		eye = eye + translation;
		if(lockLookat) {
			lookAt = (target - eye);//.normalized();
			third = lookAt.normalized().crossProduct(up);
			up = third.crossProduct(lookAt.normalized());
		}
		
	}
	else {
		Vector3f target = eye + lookAt;

		eye = eye + v;
		if(lockLookat) {
			lookAt = (target - eye);//.normalized();
			third = lookAt.normalized().crossProduct(up);
			up = third.crossProduct(lookAt.normalized());
		}
		target = eye + lookAt;
	}
}

void Camera::rotateBase(const Vector3f& v, const float degrees) {
	Quaternion q(v, degrees);
	lookAt = q.rotate(lookAt);
	third = q.rotate(third);
	up = q.rotate(up);
}

void Camera::look() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vector3f focusPos = eye + lookAt;
	gluLookAt(eye.getX(),       eye.getY(),       eye.getZ(),
			   focusPos.getX(),  focusPos.getY(),  focusPos.getZ(),
			   up.getX(),        up.getY(),        up.getZ());
}
