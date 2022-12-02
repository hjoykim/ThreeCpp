#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H
#include <three/utils/stb_image.h>
#include <three/textures/Texture.h>
#include <string>
namespace three {
	class TextureLoader {
	public:
		TextureLoader(){}
		static Texture::ptr load(const string& filePath) {
			
			Texture::ptr texture = nullptr;

			int width, height, nrComponents;

			unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
			std::vector<unsigned char> image(width * height * nrComponents);
			
			if (data) {
				std::memcpy(&image[0], data, width * height * nrComponents);
				delete[] data;
				PixelFormat format = PixelFormat::None;
				if (nrComponents == 1)
					format = PixelFormat::RedFormat;
				else if (nrComponents == 3)
					format = PixelFormat::RGBFormat;
				else if (nrComponents == 4)
					format = PixelFormat::RGBAFormat;

				texture = Texture::create(image);
				texture->format = format;
				texture->imageWidth = width;
				texture->imageHeight = height;
				texture->setNeedsUpdate(true);
				texture->channel = nrComponents;
			}
			return texture;
			
		}
	};
}
#endif