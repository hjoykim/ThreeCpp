#ifndef GLBUFFERED_RENDERER_THREE_H
#define GLBUFFERED_RENDERER_THREE_H
#include <three/renderers/gl/GLBufferRendererInterface.h>
namespace three {
	class GLRenderer;
	namespace gl {		

		class GLBufferRenderer : public GLBufferRendererInterface {
			
		public:	

			
			GLBufferRenderer(bool isGL2 = false) : GLBufferRendererInterface(isGL2) {}

			GLBufferRenderer(GLExtensions::ptr& extension, GLInfo::ptr& info, GLCapabilities::ptr& capabilities)
				: GLBufferRendererInterface(extension,info, capabilities){}			

			virtual void setMode(const GLenum value) override {
				mode = value;
			}
			
			virtual void render(const GLint start, const GLsizei count) override {
				glDrawArrays(mode, start, count);				
				info->update(count, (unsigned)mode);
				GLBufferRendererInterface::render(start, count);
			}

			virtual void renderInstances(const Geometry& geometry,const GLint start,const GLsizei count,const GLint primcount) override {
				if (primcount == 0) return;

				glDrawArraysInstanced(mode, start, count, primcount);

				info->update(count, (int)mode, primcount);
				GLBufferRendererInterface::renderInstances(geometry,start, count,primcount);
 			}

			virtual GLBufferRenderer& copy(const GLBufferRenderer& source)  {
				GLBufferRendererInterface::copy(source);				
				return *this;
			}
			virtual GLBufferRenderer& operator = (const GLBufferRenderer& source) {
				return copy(source);
			}
		};
	}
}
#endif