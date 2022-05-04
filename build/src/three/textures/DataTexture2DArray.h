#ifndef DATA_TEXTURE_2DARRAY_THREE_H
#define DATA_TEXTURE_2DARRAY_THREE_H
#include <three/textures/Texture.h>
namespace three {
	class DataTexture2DArray : public Texture {
	public:
		DataTexture2DArray() :Texture() {
			isDataTexture2DArray = true;
		}
	};
}
#endif