#ifndef GLINDEXED_BUFFERED_RENDERER_THREE_H
#define GLINDEXED_BUFFERED_RENDERER_THREE_H

#include <three/renderers/gl/GLBufferRendererInterface.h>
namespace three {
	namespace gl {
		class GLIndexedBufferRenderer :public GLBufferRendererInterface{
		
		public :
			GLIndexedBufferRenderer(bool isGL2 = false) :GLBufferRendererInterface(isGL2) {}
			GLIndexedBufferRenderer(GLExtensions::ptr& extension, GLInfo::ptr& info, GLCapabilities::ptr& capabilities)
				: GLBufferRendererInterface(extension, info, capabilities) {}

			virtual void setMode(const GLenum value) override {
				mode = value;
			}

			virtual void setIndex(Attribute& value)  override {
				type = value.type;
				bytesPerElement = value.bytesPerElement;
			}
			virtual void render(const GLint start, const GLsizei count) override {
				glDrawElements(mode, count, type, (const size_t *)(start * bytesPerElement));
				info->update(count, mode);
				GLBufferRendererInterface::render(start,count);
			}

			virtual void renderInstances(const Geometry& geometry, const GLint start, const GLsizei count, const GLint primcount) override {
				glDrawElementsInstanced(mode, count, type, (const size_t*)(start * bytesPerElement), primcount);
				info->update(count, mode, primcount);
				GLBufferRendererInterface::renderInstances(geometry,start, count,primcount);
			}
			GLIndexedBufferRenderer& copy(const GLIndexedBufferRenderer& source) {
				GLBufferRendererInterface::copy(source);
				return *this;
			}
			GLIndexedBufferRenderer& operator = (const GLIndexedBufferRenderer& source) {
				return copy(source);
			}
		};
	}
}
#endif
