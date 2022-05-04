#ifndef ORTHOGRAPHICCAMERA_THREE_H
#define ORTHOGRAPHICCAMERA_THREE_H
#include <three/cameras/Camera.h>
#include <limits>
namespace three {

	class OrthographicCamera : public Camera {
	public:
	

		CameraView view;

		using ptr = std::shared_ptr<OrthographicCamera>;
			
		OrthographicCamera(float left=std::numeric_limits<float>::quiet_NaN(), float right= std::numeric_limits<float>::quiet_NaN(), float top= std::numeric_limits<float>::quiet_NaN(), float bottom= std::numeric_limits<float>::quiet_NaN(), float _near= std::numeric_limits<float>::quiet_NaN(), float _far= std::numeric_limits<float>::quiet_NaN()) : Camera() {
			
			type = "OrthographicCamera";			
			isOrthographic = true;

			zoom = 1;

			this->left = !std::isnan(left)  ? left : -1;
			this->cameraRight = !std::isnan(right)  ? right : 1;
			this->top = !std::isnan(top)  ? top : 1;
			this->bottom = !std::isnan(bottom)  ? bottom : -1;

			this->_near = std::isnan(_near)  ? _near : 0.1f;
			this->_far = std::isnan(_far)  ? _far : 2000;

			view.enabled = false;

			this->updateProjectionMatrix();

		}
		OrthographicCamera(OrthographicCamera& source) : Camera(source) {
			this->zoom = source.zoom;
			isOrthographic = true;
			this->left = source.left;
			this->cameraRight = source.cameraRight;
			this->top = source.top;
			this->bottom = source.bottom;		
			this->view = source.view;
		}

		OrthographicCamera& copy(OrthographicCamera& source) {
			Camera::copy(source);
			this->zoom = source.zoom;

			this->left = source.left;
			this->cameraRight = source.cameraRight;
			this->top = source.top;
			this->bottom = source.bottom;
			this->view = source.view;
		}
		

		static ptr create(float left = std::numeric_limits<float>::quiet_NaN(), float right = std::numeric_limits<float>::quiet_NaN(), float top = std::numeric_limits<float>::quiet_NaN(), float bottom = std::numeric_limits<float>::quiet_NaN(), float _near = std::numeric_limits<float>::quiet_NaN(), float _far = std::numeric_limits<float>::quiet_NaN()) {
			return ptr(new OrthographicCamera(left, right, top, bottom, _near, _far));
		}

		void setViewOffset(float fullWidth, float fullHeight, float x, float y, float width, float height) override {
			view.enabled = true;
			view.fullWidth = fullWidth;
			view.fullHeight = fullHeight;
			view.offsetX = x;
			view.offsetY = y;
			view.width = width;
			view.height = height;

			this->updateProjectionMatrix();
		}
		void clearViewOffset() override {
			this->view.enabled = false;

			this->updateProjectionMatrix();
		}
		virtual OrthographicCamera* clone() {
			return new OrthographicCamera(*this);
		}
		virtual void updateProjectionMatrix() override {
			auto dx = (this->cameraRight - this->left) / (2 * this->zoom);
			auto dy = (this->top - this->bottom) / (2 * this->zoom);
			auto cx = (this->cameraRight + this->left) / 2;
			auto cy = (this->top + this->bottom) / 2;

			auto left = cx - dx;
			auto right = cx + dx;
			auto top = cy + dy;
			auto bottom = cy - dy;

			if (this->view.enabled) {

				auto zoomW = this->zoom / (this->view.width / this->view.fullWidth);
				auto zoomH = this->zoom / (this->view.height / this->view.fullHeight);
				auto scaleW = (this->cameraRight - this->left) / this->view.width;
				auto scaleH = (this->top - this->bottom) / this->view.height;

				left += scaleW * (this->view.offsetX / zoomW);
				right = left + scaleW * (this->view.width / zoomW);
				top -= scaleH * (this->view.offsetY / zoomH);
				bottom = top - scaleH * (this->view.height / zoomH);

			}

			this->projectionMatrix = Matrix4().makeOrthographic(left, right, top, bottom, this->_near, this->_far);

			this->projectionMatrixInverse.getInverse(this->projectionMatrix);
		}
	};


}
#endif
