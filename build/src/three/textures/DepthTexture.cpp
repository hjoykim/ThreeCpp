#include "DepthTexture.h"
namespace three {
	DepthTexture::DepthTexture(size_t width, size_t height, TextureMapping mapping, Wrapping wraps, Wrapping wrapt, TextureFilter magFilter, unsigned anisotropy, PixelFormat format)
		:Texture(std::vector<unsigned char>(),mapping,wraps,wrapt,magFilter, TextureFilter::NearestFilter,PixelFormat::None,TextureDataType::UnsignedByteType,anisotropy),width(width),height(height)
	{
		isDepthTexture = true;
		format = format != PixelFormat::None ? format : PixelFormat::DepthFormat;
		assert(format == PixelFormat::DepthFormat || format == PixelFormat::DepthStencilFormat);

		if ((unsigned)type==0 && format == PixelFormat::DepthFormat) type = TextureDataType::UnsignedShortType;
		if ((unsigned)type == 0 && format == PixelFormat::DepthStencilFormat) type = TextureDataType::UnsignedInt248Type;

		//image = byte[width][height];

		flipY = false;
		generateMipmaps = false;

	}
}
