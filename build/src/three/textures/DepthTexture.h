#ifndef DEPTH_TEXTURE_THREE_H
#define DEPTH_TEXTURE_THREE_H
#include <three/textures/Texture.h>
#include <three/Constants.h>
namespace three {
	class DepthTexture : public Texture {
	private:
		size_t width, height;
	public:
		using ptr = std::shared_ptr<DepthTexture>;

		DepthTexture() :Texture(),width(0), height(0) {
			isDepthTexture = true;
		}

		DepthTexture(size_t width, size_t height,
			TextureMapping mapping = TextureMapping::UVMapping,
			Wrapping wraps = Wrapping::ClampToEdgeWrapping,
			Wrapping wrapt = Wrapping::ClampToEdgeWrapping,
			TextureFilter magFilter = TextureFilter::NearestFilter,
			unsigned anisotropy = 1,
			PixelFormat  format = PixelFormat::None);
	};
}
#endif
