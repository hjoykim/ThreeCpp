#include "GLCubeMap.h"
#include <three/renderers/GLRenderer.h>
namespace three {

	namespace gl {
		GLCubeMap::GLCubeMap(GLRenderer& renderer) : renderer(renderer)
		{
		}

		Texture::ptr GLCubeMap::get(const Texture::ptr texture)
		{
			return Texture::ptr();
		}

	}
}