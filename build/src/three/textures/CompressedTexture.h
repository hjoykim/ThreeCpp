#ifndef COMPRESSED_TEXTURE_THREE_H
#define COMPRESSED_TEXTURE_THREE_H
#include <three/textures/Texture.h>
namespace three {
	class CompressedTexture : public Texture {
		CompressedTexture(std::vector<MipMap> mipmaps, unsigned width = 0, unsigned height = 0, TextureMapping mapping = TextureMapping::UVMapping, Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping, TextureFilter magFilter = TextureFilter::NearestFilter, TextureFilter minFilter = TextureFilter::NearestFilter, PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType, unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding)
			:Texture(std::vector<unsigned char>(), mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding) 
		{
			this->imageWidth = width;
			this->imageHeight = height;
			this->generateMipmaps = false;
			this->flipY = false;			
		}
	};
}
#endif