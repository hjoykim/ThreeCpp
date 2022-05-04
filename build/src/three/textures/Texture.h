#ifndef TEXTURE_THREE_H
#define TEXTURE_THREE_H

#include <three/utils/sole.h>
#include <three/Constants.h>
#include <string>
#include <vector>
#include <three/math/Vector2.h>
#include <three/math/Matrix3.h>
#include <three/utils/simplesignal.h>
#include <three/Types.h>
#include <sstream>
using namespace three::math;
namespace three {
	
	class Texture {
		static size_t textureId;
		
	private:
		

	public:

		using ptr = std::shared_ptr<Texture>;

		bool needsUpdate;

		size_t id;

		sole::uuid uuid = sole::uuid1();

		std::string name;

		std::string sourceFile;

		std::vector<unsigned char> image;	

		std::vector<Texture> images;

		GLsizei imageWidth;

		GLsizei imageHeight;

		GLsizei depth;

		int channel;
		//std::vector<unsigned char> image;

		std::vector<MipMap> mipmaps;

		TextureMapping mapping;

		Wrapping wrapS;

		Wrapping wrapT;

		Wrapping wrapR = Wrapping::ClampToEdgeWrapping;

		TextureFilter magFilter;

		TextureFilter minFilter;

		unsigned anisotropy;

		PixelFormat format;

		PixelFormatGPU internalFormat;

		TextureDataType type;

		Vector2 offset;

		Vector2 repeat;

		Vector2 center;

		float rotation;

		bool generateMipmaps;
		
		bool premultiplyAlpha;
		
		bool flipY;

		unsigned unpackAlignment;
		
		TextureEncoding encoding;
		
		size_t version = 0;


		bool matrixAutoUpdate;

		Matrix3 matrix;

		bool isCompressedTexture = false;
		bool isCubeTexture = false;
		bool isDataTexture = false;
		bool isDataTexture3D = false;
		bool isDataTexture2DArray = false;
		bool isDepthTexture = false;
		bool isVideoTexture = false;

		Signal<void(Texture&)> onDispose;
		Signal<void(Texture&)> onUpdate;

		Texture(std::vector<unsigned char> image=std::vector<unsigned char>(), TextureMapping mapping = TextureMapping::UVMapping, Wrapping wraps = Wrapping::ClampToEdgeWrapping, Wrapping wrapt = Wrapping::ClampToEdgeWrapping, TextureFilter magFilter = TextureFilter::LinearFilter, TextureFilter minFilter = TextureFilter::LinearMipMapLinearFilter, PixelFormat format = PixelFormat::RGBAFormat, TextureDataType type = TextureDataType::UnsignedByteType, unsigned anisotropy = 1, TextureEncoding encoding = TextureEncoding::LinearEncoding);

		Texture(const Texture& source);

		static ptr create(std::vector<unsigned char> image = std::vector<unsigned char>(),
			TextureMapping mapping = TextureMapping::UVMapping, 
			Wrapping wraps = Wrapping::ClampToEdgeWrapping, 
			Wrapping wrapt = Wrapping::ClampToEdgeWrapping, 
			TextureFilter magFilter = TextureFilter::LinearFilter, 
			TextureFilter minFilter = TextureFilter::LinearMipMapLinearFilter, 
			PixelFormat format = PixelFormat::RGBAFormat, 
			TextureDataType type = TextureDataType::UnsignedByteType, 
			unsigned anisotropy = 1, 
			TextureEncoding encoding = TextureEncoding::LinearEncoding) {
			return std::make_shared<Texture>(image, mapping, wraps, wrapt, magFilter, minFilter, format, type, anisotropy);
		}

		virtual ~Texture() = default;
		
		
		void updateMatrix() {
			matrix.setUvTransform(offset.x, offset.y, repeat.x, repeat.y, rotation, center.x, center.y);
		}
		Texture* clone();

		Texture& copy(const Texture& source);
		
		Vector2& transformUv(Vector2* uv);

		void updateTexture();

		bool isEmpty()  const{
			return id == std::numeric_limits<size_t>::infinity() || id == 0;
		}

		void setNeedsUpdate(const bool value) {
			if (value == true) {
				version++;
				needsUpdate = value;
			}
		}
	};
}
#endif