#ifndef GLMULTISAMPLE_RENDER_TARGET_THREE_H
#define GLMULTISAMPLE_RENDER_TARGET_THREE_H
#include <three/renderers/gl/GLRenderTarget.h>
namespace three {
	namespace gl {
		class GLMultisampleRenderTarget : public GLRenderTarget {
		public:
			GLMultisampleRenderTarget() :GLRenderTarget() {
				type = "GLMultisampleRenderTarget";
				isGLMultisampleRenderTarget = true;
			}
		};
	}
}
#endif