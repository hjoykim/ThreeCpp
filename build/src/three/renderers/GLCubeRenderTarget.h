#ifndef GLCUBE_RENDER_TARGET_THREE_H
#define GLCUBE_RENDER_TARGET_THREE_H
#include <three/renderers/gl/GLRenderTarget.h>
namespace three {
	namespace gl {
		class GLCubeRenderTarget : public GLRenderTarget {
		public:
			GLCubeRenderTarget() : GLRenderTarget() {
				type = "GLCubeRenderTarget";
			}
		};
	}
}
#endif
