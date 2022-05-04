#ifndef GLCUBE_MAP_THREE_H
#define GLCUBE_MAP_THREE_H
#include <three/textures/Texture.h>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
namespace three {
	class GLRenderer;
	namespace gl {
		class GLCubeMap {
		private:
			GLRenderer& renderer;
			std::unordered_map<std::string, Texture::ptr> cubemaps;
		public:
			using ptr = std::shared_ptr<GLCubeMap>;

			GLCubeMap(GLRenderer& renderer);

			static ptr create(GLRenderer& renderer) {
				return std::make_shared<GLCubeMap>(renderer);
			}

			~GLCubeMap() = default;

			Texture::ptr get(const Texture::ptr texture);
		};
	}
}

#endif