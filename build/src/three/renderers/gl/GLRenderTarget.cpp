#include "GLRenderTarget.h"
#include <three/Constants.h>
#include <three/Types.h>
#include <three/math/Vector4.h>
namespace three {
	namespace gl {
		size_t GLRenderTarget::renderTargetId = 0;

		GLRenderTarget::GLRenderTarget(size_t width, size_t height ,GLRenderTargetParameter* options) : GLRenderTarget()
		{
			type = "GLRenderTarget";
			this->width = width;
			this->height = height;

			scissor.set(0, 0, (float)width, (float)height);
			scissorTest = false;

			viewport.set(0, 0, (float)width, (float)height);

			if (options != nullptr) {

				//this->options = *options;				
				//std::copy(options->begin(), options->end(), this->options.begin());
				TextureMapping mapping = options->mapping;
				Wrapping wraps = options->wrapS;
				Wrapping wrapt = options->wrapT;
				TextureFilter magFilter = options->magFilter;
				TextureFilter minFilter = options->minFilter;
				PixelFormat format = options->format;
				TextureDataType type = options->type;
				int anisotropy = options->anisotropy;
				TextureEncoding encoding = options->encoding;
					
				texture = std::make_shared<Texture>(std::vector<unsigned char>(),
					mapping, 
					wraps, 
					wrapt,
					magFilter, 
					minFilter, 
					format,
					type, 
					(unsigned)anisotropy, 
					encoding);

				texture->imageWidth = width;
				texture->imageHeight = height;

				texture->generateMipmaps = options->generateMipmaps;
				texture->minFilter = options->minFilter;
				depthBuffer = options->depthBuffer;
				stencilBuffer = options->stencilBuffer;
				depthTexture = options->depthTexture;
			}

		}
		GLRenderTarget::GLRenderTarget(GLRenderTarget& source) : GLRenderTarget()
		{
			width = source.width;
			height = source.height;
			scissor.copy(source.scissor);
			scissorTest = source.scissorTest;
			viewport.copy(source.viewport);
			if (texture != nullptr) {
				texture.reset();
				texture = source.texture;
			}
			if (depthTexture != nullptr) {
				depthTexture.reset();
				depthTexture = source.depthTexture;
			}
			depthBuffer = source.depthBuffer;
			stencilBuffer = source.stencilBuffer;
			//options.merge(source.options);
			samples = source.samples;

		}
		void GLRenderTarget::setSize(size_t width, size_t height)
		{
			if (this->width != width || this->height != height) {
				this->width = width;
				this->height = height;

				this->dispose();
			}
			this->viewport.set(0, 0, (float)width, (float)height);
			this->scissor.set(0, 0, (float)width, (float)height);
		}
		GLRenderTarget& GLRenderTarget::clone(GLRenderTarget* target)
		{
			target->copy(*this);

			return *target;
		}
		GLRenderTarget& GLRenderTarget::copy(const GLRenderTarget& source)
		{
			type = "GLRenderTarget";
			width = source.width;
			height = source.height;
			scissor.copy(source.scissor);
			scissorTest = source.scissorTest;
			viewport.copy(source.viewport);
			if (texture != nullptr) {
				texture.reset();
				texture->copy(*source.texture);
			}
			if (depthTexture != nullptr) {
				depthTexture.reset();
				depthTexture->copy(*source.depthTexture);
			}
			depthBuffer = source.depthBuffer;
			stencilBuffer = source.stencilBuffer;
			//options.merge(source.options);
			samples = source.samples;

			return *this;
		}
		void GLRenderTarget::dispose()
		{
			// must define to delete texture, and depthTexture
			onDispose.emitSignal(*this);
			onDispose.disconnectAll();
		}
	}
}
	