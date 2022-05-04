#ifndef CUBE_TEXTURE_THREE_H
#define CUBE_TEXTURE_THREE_H
#include <three/textures/Texture.h>
namespace three {
	class CubeTexture : public Texture {
	public:
		using ptr = std::shared_ptr<CubeTexture>;
		CubeTexture(std::vector<unsigned char> image = std::vector<unsigned char>(), TextureMapping mapping = TextureMapping::CubeReflectionMapping, Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping, TextureFilter magFilter = TextureFilter::LinearFilter, TextureFilter minFilter = TextureFilter::LinearMipMapLinearFilter, PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType, unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding)
			:Texture(image, mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding) {
			flipY = false;
		}

	};
}
#endif