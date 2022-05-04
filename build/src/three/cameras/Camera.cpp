#include "Camera.h"
namespace three {

	Camera::Camera() : Object3D()
	{
		type = "Camera";
		isCamera = true;
	}
	Camera::Camera(const Camera& camera) : Object3D(camera) {
		type = "Camera";
		fov = camera.fov;
		aspect = camera.aspect;
		_far = camera._far;
		_near = camera._near;
		zoom = camera.zoom;
		matrixWorldInverse = camera.matrixWorldInverse;
		projectionMatrix = camera.projectionMatrix;
		projectionMatrixInverse = camera.projectionMatrixInverse;

		isCamera = true;
	}
	Vector3& Camera::getWorldDirection(math::Vector3& target)
	{
		// TODO: 여기에 return 문을 삽입합니다.
		this->updateMatrixWorld(true);

		float* e = matrixWorld.elements;

		return target.set(-e[8], -e[9], -e[10]).normalize();

	}
	void Camera::updateMatrixWorld(bool force)
	{
		Object3D::updateMatrixWorld(force);

		matrixWorldInverse.getInverse(this->matrixWorld);
	}
	void Camera::updateWorldMatrix(bool updateParents, bool updateChildren)
	{
		Object3D::updateWorldMatrix(updateParents, updateChildren);

		matrixWorldInverse.getInverse(this->matrixWorld);
	}

	Camera& Camera::copy(Camera& source, bool recursive) {

		Object3D::copy(source, recursive);
		fov = source.fov;
		aspect = source.aspect;
		_far = source._far;
		_near = source._near;
		zoom = source.zoom;
		matrixWorldInverse = source.matrixWorldInverse;
		projectionMatrix = source.projectionMatrix;
		projectionMatrixInverse = source.projectionMatrixInverse;
		isCamera = true;

		return *this;
	}

	Camera* Camera::clone()
	{
		Camera* camera = new Camera();
		camera->copy(*this);
		return camera;
	}

	void Camera::updateProjectionMatrix()
	{
		matrixWorldInverse.getInverse(matrixWorld);
	}
	
	
}
