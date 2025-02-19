#include "camera.h"

Camera::Camera(float fov, float near, float far, QVector3D center)
{
	this->front = QVector3D(0.0f, 0.0f, 1.0f);
	this->up = QVector3D(0.0f, 1.0f, 0.0);
	this->center = center;

	this->distance = 10.0f;
	this->fov = fov;
	this->speed = 0.5;

	this->nearPlane = near;
	this->farPlane = far;
}

Camera::Camera(CameraSettings settings, float fov, float near, float far)
{
	this->front = settings.front;
	this->up = settings.up;
	this->center = settings.center;

	this->distance = settings.distance;
	this->fov = fov;
	this->speed = 0.5;

	this->nearPlane = near;
	this->farPlane = far;

	this->position = settings.position;
	this->translate = settings.translate;

	this->x_rotation = settings.x_rotation;
	this->y_rotation = settings.y_rotation;
	this->z_rotation = settings.z_rotation;
}

Camera::~Camera()
{
}

void Camera::frameUpdate()
{
	this->model.setToIdentity();

	this->model.rotate(180.0f - (x_rotation / 16.0f), 1, 0, 0);
	this->model.rotate(y_rotation / 16.0f, 0, 1, 0);
	this->model.rotate(z_rotation / 16.0f, 0, 0, 1);

	this->model.translate(-center);

	this->position = -this->front * this->distance;
	this->view.setToIdentity();
	this->view.translate(position);
	this->view.translate(translate);
}

void Camera::setAspectRatio(qreal aspect_ratio)
{
	this->projection.setToIdentity();
	this->projection.perspective(this->fov, aspect_ratio, this->nearPlane, this->farPlane);
}

QVector3D Camera::getPosition()
{
	return this->model.inverted() * this->position;
}

void Camera::resetCamera()
{
}

void Camera::mouse_press_event(QMouseEvent* event)
{
	last_mouse_position = event->pos();
}

void Camera::mouse_move_event(QMouseEvent* event)
{
	int dx = event->x() - last_mouse_position.x();
	int dy = event->y() - last_mouse_position.y();
	int delta = 8;

	// rotate the camera
	if (event->buttons() == Qt::LeftButton)
	{
		if (event->buttons() & Qt::LeftButton) {
			setXRotation(x_rotation + delta * dy);
			setYRotation(y_rotation + delta * dx);
		}
		else if (event->buttons() & Qt::RightButton) {
			setXRotation(x_rotation + delta * dy);
			setZRotation(z_rotation + delta * dx);
		}
		last_mouse_position = event->pos();
	}
	// pan the camera
	else if (event->buttons() == Qt::RightButton)
	{
		float slow_down_factor = 10000.0;
		translate = QVector3D(translate.x() + (dx * this->distance / slow_down_factor), translate.y() - dy * this->distance / slow_down_factor, 0.0);
	}
}

void Camera::processScroll(double delta)
{
	if (delta < 0)
	{
		this->distance *= 0.9;
	}
	else
	{
		this->distance *= 1.1;
	}
}

int Camera::normalize_angle(int angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;

	return angle;
}

void Camera::setXRotation(int angle)
{
	angle = normalize_angle(angle);
	if (angle != x_rotation)
	{
		x_rotation = angle;
	}
}

void Camera::setYRotation(int angle)
{
	angle = normalize_angle(angle);
	if (angle != y_rotation)
	{
		y_rotation = angle;
	}
}

void Camera::setZRotation(int angle)
{
	angle = normalize_angle(angle);
	if (angle != z_rotation)
	{
		z_rotation = angle;
	}
}

QMatrix4x4& Camera::getModelMatrix()
{
	return this->model;
}

QMatrix4x4& Camera::getViewMatrix()
{
	return this->view;
}

QMatrix4x4& Camera::getProjectionMatrix()
{
	return this->projection;
}

CameraSettings Camera::getCameraSettings()
{
	CameraSettings settings;

	settings.center = this->center;
	settings.distance = this->distance;
	settings.front = this->front;
	settings.position = this->position;
	settings.translate = this->translate;
	settings.up = this->up;

	settings.x_rotation = this->x_rotation;
	settings.y_rotation = this->y_rotation;
	settings.z_rotation = this->z_rotation;

	return settings;
}