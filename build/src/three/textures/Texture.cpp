#include "Texture.h"

namespace three {
	size_t Texture::textureId = 0;
	Texture::Texture(std::vector<unsigned char> image,TextureMapping mapping,Wrapping wraps,Wrapping wrapt,TextureFilter magFilter,TextureFilter minFilter, PixelFormat format,TextureDataType type, unsigned anisotropy, TextureEncoding encoding)
	{
		id = textureId++;

		name = "";

		this->image = image;

		internalFormat = PixelFormatGPU::NONE;

		this->format = format;

		offset.set(0, 0);

		repeat.set(1, 1);

		center.set(0, 0);

		rotation = 0;

		matrixAutoUpdate = true;

		generateMipmaps = true;

		premultiplyAlpha = false;

		flipY = true;

		unpackAlignment = 4;

		version = 0;

		this->mapping = mapping;
		this->wrapS = wraps;
		this->wrapT = wrapt;
		this->magFilter = magFilter;
		this->minFilter = minFilter;
		this->format = format;
		this->type = type;
		this->anisotropy = anisotropy;
		this->encoding = encoding;
	}
	Texture::Texture(const Texture& source) {
		name = source.name;

		image = source.image;

		mipmaps = source.mipmaps;

		mapping = source.mapping;

		wrapS = source.wrapS;
		wrapT = source.wrapT;

		magFilter = source.magFilter;
		minFilter = source.minFilter;

		anisotropy = source.anisotropy;

		format = source.format;
		internalFormat = source.internalFormat;
		type = source.type;

		offset.copy(source.offset);
		repeat.copy(source.repeat);
		center.copy(source.center);
		rotation = source.rotation;

		//TODO copy images

		matrixAutoUpdate = source.matrixAutoUpdate;
		matrix.copy(source.matrix);

		generateMipmaps = source.generateMipmaps;
		premultiplyAlpha = source.premultiplyAlpha;
		flipY = source.flipY;
		unpackAlignment = source.unpackAlignment;
		encoding = source.encoding;
	}
	Texture* Texture::clone()
	{
		return new Texture(*this);
	}

	Texture& Texture::copy(const Texture& source)
	{
		name = source.name;

		image = source.image;

		mipmaps = source.mipmaps;

		mapping = source.mapping;

		wrapS = source.wrapS;
		wrapT = source.wrapT;

		magFilter = source.magFilter;
		minFilter = source.minFilter;

		anisotropy = source.anisotropy;

		format = source.format;
		internalFormat = source.internalFormat;
		type = source.type;

		offset.copy(source.offset);
		repeat.copy(source.repeat);
		center.copy(source.center);
		rotation = source.rotation;
		
		//TODO copy images

		matrixAutoUpdate = source.matrixAutoUpdate;
		matrix.copy(source.matrix);

		generateMipmaps = source.generateMipmaps;
		premultiplyAlpha = source.premultiplyAlpha;
		flipY = source.flipY;
		unpackAlignment = source.unpackAlignment;
		encoding = source.encoding;

		return *this;
	}

	Vector2& Texture::transformUv(Vector2* uv)
	{
		if (mapping != TextureMapping::UVMapping) return *uv;

		uv->applyMatrix3(matrix);

		if (uv->x < 0 || uv->x > 1) {

			switch (wrapS) {

			case Wrapping::RepeatWrapping:

				uv->x = uv->x - std::floor(uv->x);
				break;

			case Wrapping::ClampToEdgeWrapping:

				uv->x = uv->x < 0 ? 0.0f : 1.0f;
				break;

			case Wrapping::MirroredRepeatWrapping:

				if (std::abs(std::fmod(std::floor(uv->x), 2)) ==  1) {

					uv->x = ceil(uv->x) - uv->x;

				}
				else {

					uv->x = uv->x - floor(uv->x);

				}
				break;

			}

		}

		if (uv->y < 0 || uv->y > 1) {

			switch (wrapT) {

			case Wrapping::RepeatWrapping:

				uv->y = uv->y - floor(uv->y);
				break;

			case Wrapping::ClampToEdgeWrapping:

				uv->y = uv->y < 0 ? 0.0f : 1.0f;
				break;

			case Wrapping::MirroredRepeatWrapping:

				if (abs(std::fmod(floor(uv->y),2)) == 1) {

					uv->y = ceil(uv->y) - uv->y;

				}
				else {

					uv->y = uv->y - floor(uv->y);

				}
				break;

			}

		}

		if (flipY) {

			uv->y = 1 - uv->y;

		}

		return *uv;
	}
	void Texture::updateTexture()
	{
		
	}
}
