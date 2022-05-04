#ifndef VIDEO_TEXTURE_THREE_H
#define VIDEO_TEXTURE_THREE_H
#include <three/textures/Texture.h>
namespace three {

	class VideoTexture : public Texture {
		VideoTexture(std::vector<unsigned char> image = std::vector<unsigned char>(), TextureMapping mapping = TextureMapping::UVMapping, Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping, TextureFilter magFilter = TextureFilter::LinearFilter, TextureFilter minFilter = TextureFilter::LinearFilter, PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType, unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding)
			:Texture(image, mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy, encoding) {
			generateMipmaps = false;
		}
		
		void update() {
			if (this->image.size() > 0) {
				this->setNeedsUpdate(true);
			}
		}			
	};
}
#endif