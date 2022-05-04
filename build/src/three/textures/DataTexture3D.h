#ifndef DATA_TEXTURE_3D_THREE_H
#define DATA_TEXTURE_3D_THREE_H
#include <three/textures/Texture.h>
namespace three {
	class DataTexture3D : public Texture {
	public:
		DataTexture3D() :Texture() 
		{
			isDataTexture3D = true;
		}
	};
}
#endif
