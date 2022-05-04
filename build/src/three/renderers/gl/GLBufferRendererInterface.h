#ifndef GLRENDERER_INTERFACE_THREE_H
#define GLRENDERER_INTERFACE_THREE_H
#include <memory>
#include <three/renderers/gl/GLExtensions.h>
#include <three/renderers/gl/GLCapabilities.h>
#include <three/renderers/gl/GLInfo.h>
#include <three/core/Attribute.h>
#include <three/core/Geometry.h>
#include <iostream>
#include <string>
namespace three {
	namespace gl {
		class GLBufferRendererInterface {

		public :
			bool isGL2 ;

			GLExtensions::ptr extensions;

			GLInfo::ptr info;

			GLCapabilities::ptr capabilities;

			unsigned type = 0;

			unsigned bytesPerElement = 4;

			GLenum mode = 0;

			using ptr = std::shared_ptr<GLBufferRendererInterface>;
					
			GLBufferRendererInterface(bool isGL2 = false) : isGL2(isGL2) {}
			GLBufferRendererInterface(GLExtensions::ptr& extension, GLInfo::ptr& info, GLCapabilities::ptr& capabilities)
				:extensions(extensions), info(info), capabilities(capabilities) {
				isGL2 = capabilities->isGL2;
			}
			virtual ~GLBufferRendererInterface() = default;

			virtual void setMode(const GLenum value) {}

			virtual void setIndex(Attribute& value) {}

			virtual void render(const GLint start, const GLsizei count) 
			{				
				checkError();
			}

			virtual void renderInstances(const Geometry& geometry, const GLint start, const GLsizei count, const GLint primcount) {
				checkError();
			}
			
			virtual GLBufferRendererInterface& copy(const GLBufferRendererInterface& source) {
				isGL2 = source.isGL2;
				mode = source.mode;
				type = source.type;
				bytesPerElement = source.bytesPerElement;
				extensions = source.extensions;
				info = source.info;
				capabilities = source.capabilities;
				return *this;
			}
			
			virtual GLBufferRendererInterface& operator = (const GLBufferRendererInterface& source) {
				return copy(source);
			}
		};
	}
}
#endif