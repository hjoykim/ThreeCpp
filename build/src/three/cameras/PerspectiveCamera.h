#ifndef PERSPECTIVE_CAMERA_THREE_H
#define PERSPECTIVE_CAMERA_THREE_H

#include <three/cameras/Camera.h>
#include <limits>
namespace three {

	class PerspectiveCamera : public Camera {
	public:
		

		using ptr = std::shared_ptr<PerspectiveCamera>;



		CameraView view = CameraView();

		float focus = 10;

		float filmGauge = 35;

		float filmOffset = 0;

		PerspectiveCamera() : Camera() {
			isPerspective = true;
		}

		PerspectiveCamera(float fov, float aspect, float _near = std::numeric_limits<float>::quiet_NaN(), float _far=std::numeric_limits<float>::quiet_NaN()) : Camera() 
		{
			isPerspective = true;
			this->fov = !fov ? fov : 50.0f;
			this->zoom = 1;
			this->_near = !std::isnan(_near) ? _near : 0.1f;
			this->_far = !std::isnan(_far) ? _far : 2000.0f;

			updateProjectionMatrix();
		}

		PerspectiveCamera(const PerspectiveCamera& camera) : Camera(camera) {
			isPerspective = true;
			view = camera.view;
			focus = camera.focus;
			filmGauge = camera.filmGauge;
			filmOffset = camera.filmOffset;

			updateProjectionMatrix();
		}

		virtual PerspectiveCamera* clone() {
			return new PerspectiveCamera(*this);
		}
		virtual PerspectiveCamera& copy(PerspectiveCamera& camera, bool recursive) {
			Camera::copy(camera, recursive);
			view = camera.view;
			focus = camera.focus;
			filmGauge = camera.filmGauge;
			filmOffset = camera.filmOffset;

			updateProjectionMatrix();

			return *this;
		}

		static ptr create(float fov, float aspect, float _near, float _far) {
			return ptr(new PerspectiveCamera(fov, aspect, _near, _far));
			//return std::make_shared<PerspectiveCamera>(fov, aspect, _near, _far);
		}
		virtual ~PerspectiveCamera() = default;

		void setFocalLength(float focalLength);

		float getFocalLength();

		float getEffectiveFOV();

		float getFilmWidth();

		float getFilmHeight();

		virtual void setViewOffset(float fullWidth, float fullHeight, float x, float y, float width, float height) override;

		virtual void clearViewOffset() override;

		virtual void updateProjectionMatrix() override;
	};

	class ArrayCamera : public PerspectiveCamera {
	public:

		

		std::vector<Camera::ptr> cameras;

		ArrayCamera() : PerspectiveCamera() {
			isArrayCamera = true;
		}

		ArrayCamera(const std::vector<Camera::ptr>& cameras) : PerspectiveCamera(), cameras(cameras) {
			isArrayCamera = true;
		}

		ArrayCamera(const ArrayCamera& source) : PerspectiveCamera(source) {
			isArrayCamera = true;
			if (source.cameras.size() > 0) {
				cameras.resize(source.cameras.size());
				std::copy(source.cameras.begin(), source.cameras.end(), cameras.begin());
			}
		}
		using ptr = std::shared_ptr<ArrayCamera>;

		static ptr create(const std::vector<Camera::ptr>& cameras) {
			return ptr(new ArrayCamera(cameras));
		}

		ArrayCamera* clone() {
			return new ArrayCamera(*this);
		}
		ArrayCamera& copy(ArrayCamera& source,bool recursive) {
			PerspectiveCamera::copy(source, recursive);
			if (source.cameras.size() > 0) {
				cameras.resize(source.cameras.size());
				std::copy(source.cameras.begin(), source.cameras.end(), cameras.begin());
			}
			isArrayCamera = true;
			return *this;
		}

	};
}
#endif