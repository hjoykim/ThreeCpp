#ifndef THREE_CAMERA_H
#define THREE_CAMERA_H
#include <three/utils/osdecl.h>
#include <three/core/Object3D.h>

namespace three {
	class Camera : public Object3D {
	
	public:
		struct CameraView {
			bool enabled;
			float fullWidth;
			float fullHeight;
			float offsetX;
			float offsetY;
			float width;
			float height;
		};
		
		float bottom;
		
		float left;
		
		float top;
		
		float zoom=1;
		
		float cameraRight;

		using ptr = std::shared_ptr<Camera>;

		/**
		* Camera frustum vertical field of view, from bottom to top of view, in degrees.
		*/
		float fov = 45.0f;
		/**
		* Camera frustum aspect ratio, window width divided by window height.
		*/
		float aspect = 1.0f;
		/**
		* Camera frustum far plane.
		*/
		float _far = 2000.0f;
		/**
		* Camera frustum near plane.
		*/
		float _near = 0.1f;

		Vector4 viewport;

		Camera();

		Camera(const Camera& camera);

		static ptr create() {
			return std::make_shared<Camera>();
		}
		/**
		 * This is the inverse of matrixWorld. MatrixWorld contains the Matrix which has the world transform of the Camera.
		 */
		Matrix4 matrixWorldInverse;
		/**
		 * This is the matrix which contains the projection.
		 */
		Matrix4 projectionMatrix;
		/**
		 * This is the inverse of projectionMatrix.
		 */
		Matrix4 projectionMatrixInverse;
		
		bool isArrayCamera = false;

		bool isPerspective = false;

		bool isOrthographic = false;

		virtual Vector3& getWorldDirection(math::Vector3& target) override;

		virtual void updateMatrixWorld(bool force = false) override;	

		virtual void updateWorldMatrix(bool updateParents = false, bool updateChildren = false) override;

		virtual void setViewOffset(float fullWidth, float fullHeight, float x, float y, float width, float height) {}

		virtual void clearViewOffset() {}

		virtual Camera& copy(Camera& source, bool recursive=true);

		virtual Camera* clone();

		virtual void updateProjectionMatrix();		
	};
}
#endif
