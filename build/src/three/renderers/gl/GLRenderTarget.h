#ifndef GLRENDER_TARGET_THREE_H
#define GLRENDER_TARGET_THREE_H
#include <string>
#include <three/utils/sole.h>
#include <three/textures/Texture.h>
#include <three/textures/DepthTexture.h>
#include <three/utils/simplesignal.h>
#include <three/math/Vector4.h>
namespace three {
	namespace gl {
		struct GLRenderTargetParameter {
			TextureMapping mapping = TextureMapping::UVMapping;
			Wrapping wrapS = Wrapping::ClampToEdgeWrapping;
			Wrapping wrapT = Wrapping::ClampToEdgeWrapping;
			TextureFilter magFilter = TextureFilter::LinearFilter;
			TextureFilter minFilter = TextureFilter::LinearMipMapLinearFilter;
			PixelFormat format = PixelFormat::RGBAFormat;
			TextureDataType type = TextureDataType::UnsignedByteType;
			int anisotropy = 1;
			TextureEncoding encoding = TextureEncoding::LinearEncoding;
			bool generateMipmaps = false;
			bool depthBuffer = true;
			bool stencilBuffer = false;
			DepthTexture::ptr depthTexture = nullptr;
		};
		class GLRenderTarget {
			static size_t renderTargetId;
		public:

			std::string type;

			size_t id;

			using ptr = std::shared_ptr<GLRenderTarget>;

			bool isGLMultiviewRenderTarget = false;

			int numViews = 0;

			sole::uuid uuid = sole::uuid1();

			size_t width = 0;

			size_t height = 0;

			math::Vector4 scissor;

			bool scissorTest = false;

			math::Vector4 viewport;

			Texture::ptr texture = nullptr;

			bool depthBuffer = false;

			bool stencilBuffer = false;

			DepthTexture::ptr depthTexture = nullptr;

			GLuint samples = 0;

			//ThreeDataTable options;
			std::unordered_map<std::string, int> options;

			bool isGLMultisampleRenderTarget = false;
			bool isGLCubeRenderTarget = false;

			GLRenderTarget() : id(renderTargetId++){ 
				type = "GLRenderTarget";
			}

			GLRenderTarget(size_t width, size_t height,GLRenderTargetParameter* options = nullptr);

			GLRenderTarget(GLRenderTarget& source);

			bool isEmpty()  const {
				return id == std::numeric_limits<size_t>::infinity() || id == 0;
			}
			virtual ~GLRenderTarget() = default;

			void setSize(size_t width, size_t height);

			GLRenderTarget& clone(GLRenderTarget* target);

			GLRenderTarget& copy(const GLRenderTarget& source);

			virtual void dispose();

			Signal<void(GLRenderTarget&)> onDispose;

			

			GLRenderTarget& operator = (const GLRenderTarget& source) {
				return copy(*this);
			}
		};
	}
}
#endif