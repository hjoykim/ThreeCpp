#ifndef GLCLIPPING_THREE_H
#define GLCLIPPING_THREE_H
#include <three/renderers/gl/GLUniforms.h>
#include <three/math/Plane.h>
#include <three/math/Matrix3.h>
#include <three/cameras/Camera.h>
#include <three/renderers/gl/GLProperties.h>
#include <limits>
using namespace three::math;
namespace three {
	namespace gl {
		class GLClipping {
		private:
			int numGlobalPlanes = 0;
			bool localClippingEnabled = false;
			bool renderingShadows = false;
			Plane plane;
			Matrix3 viewNormalMatrix;
			std::vector<float> globalState;		
			
			bool needUpdate = false;
			std::vector<float> projectPlanes(std::vector<Plane>& planes,const Camera::ptr& camera=nullptr,int dstOffset=std::numeric_limits<int>::quiet_NaN(),bool skipTransform=true);

			void resetGlobalState();
		public:
			
			int numPlanes=0;
			int numIntersection = 0;
			std::vector<float> uniform;
			using ptr = std::shared_ptr<GLClipping>;

			GLClipping() {				
			}

			bool init(std::vector<Plane>& planes, bool enableLocalClipping, const Camera::ptr& camera);

			void beginShadows();
			
			void endShadows();
			
			void setState(std::vector<Plane>& planes, bool clipIntersection, bool clipShadows, const Camera::ptr& camera, MaterialProperties& cache, bool fromCache);

		};
	}
}
#endif