#ifndef GLTEXTURES_THREE_H
#define GLTEXTURES_THREE_H
#include <three/renderers/gl/GLExtensions.h>
#include <three/renderers/gl/GLProperties.h>
#include <three/renderers/gl/GLState.h>
#include <three/renderers/gl/GLCapabilities.h>
#include <three/renderers/gl/GLInfo.h>
#include <three/utils/simplesignal.h>
namespace three {
	class Texture;

	namespace gl {

		class GLRenderTarget;

		class GLTextures {

		private:

			GLExtensions::ptr extensions;

			GLState::ptr state;

			GLProperties::ptr properties;

			GLCapabilities::ptr capabilities;

			GLInfo::ptr info;	

			bool isGL2 = false;

			GLint maxTextures;

			GLsizei maxCubemapSize = 0;

			GLsizei maxTexturesize = 0;

			GLint maxSamples = 0;

			GLuint textureUnits = 0;

			

		public:
			using ptr = std::shared_ptr<GLTextures>;
			
			GLTextures(GLExtensions::ptr& extensions, GLState::ptr& state, GLProperties::ptr& properties, GLCapabilities::ptr& capabilities, GLInfo::ptr& info);

			virtual ~GLTextures() = default;
			
			GLTextures& copy(const GLTextures& source) {
				extensions = source.extensions;
				state = source.state;
				properties = source.properties;
				capabilities = source.capabilities;
				info = source.info;
				isGL2 = source.isGL2;
				maxTextures = source.maxTextures;
				maxCubemapSize = source.maxCubemapSize;
				maxTexturesize = source.maxTexturesize;
				maxSamples = source.maxSamples;
				textureUnits = source.textureUnits;
				
				return *this;
			}
			GLTextures& operator = (const GLTextures& source) {
				return copy(source);
			}

		private:

			void initTexture(Texture& texture);

			void setTextureParameters(TextureTarget textureType, Texture& texture, bool supportsMips);

			void uploadTexture(Texture& texture, unsigned slot);

			void updateVideoTexture(Texture& texture);

			bool isPowerOfTwo(const Texture& texture);

			bool textureNeedsPowerOfTwo(Texture& texture);
			
			std::vector<unsigned char> resizeImage(std::vector<unsigned char> image, unsigned inwidth,unsigned inheight,unsigned outwidth, unsigned outheight,int nChannel);

			std::vector<unsigned char> resizeImage(std::vector<unsigned char> image,GLsizei*inwidth,GLsizei*inheight,bool needsPowerOfTwo, GLsizei maxSize,int nChannel);

			GLint getInternalFormat(PixelFormat glFormat, TextureDataType glType);

			bool textureNeedsGenerateMipmaps(Texture& texture, bool supportsMips);
			
			void generateMipmap(TextureTarget target, Texture& texture, GLsizei width, GLsizei height);

			void deallocateTexture(Texture& texture);

			GLuint getRenderTargetSamples(GLRenderTarget& renderTarget);

			void setupRenderBufferStorage(GLuint renderbuffer, GLRenderTarget& renderTarget, bool isMultisample=false);

			void setupFrameBufferTexture(GLuint framebuffer, GLRenderTarget& renderTarget, GLuint attachment, GLenum textureTarget);

			void setupDepthRenderbuffer(GLRenderTarget& renderTarget);

			void setupDepthTexture(GLuint framebuffer, GLRenderTarget& renderTarget);

			void deallocateRenderTarget(GLRenderTarget& renderTarget);
		public:
			GLuint allocateTextureUnit();

			void resetTextureUnits();

			void setTexture2D(Texture& texture, unsigned slot);

			void setTexture2DArray(Texture& texture, unsigned slot);

			void setTexture3D(Texture& texture, unsigned slot);

			void setTextureCube(Texture& texture, unsigned slot);

			void setTextureCubeDynamic(Texture& texture, unsigned slot);

			void setupRenderTarget(GLRenderTarget& renderTarget);

			void updateRenderTargetMipmap(GLRenderTarget& renderTarget);

			void updateMultisampleRenderTarget(GLRenderTarget& renderTarget);

			void safeSetTexture2D(Texture& texture, unsigned slot);

			void safeSetTextureCube(Texture& texture, unsigned slot);
						
			void onTextureDispose(Texture& texture);

			void onRenderTargetDispose(GLRenderTarget& renderTarget);
		};
	}
}
#endif