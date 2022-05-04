#ifndef PROPERTIES_THREE_H
#define PROPERTIES_THREE_H
#include <unordered_map>
#include <string>
#include <type_traits>
#include <three/utils/osdecl.h>
#include <three/Constants.h>
#include <three/utils/sole.h>
#include <three/renderers/gl/GLLights.h>
#include <three/renderers/gl/GLShader.h>
#include <three/renderers/gl/GLUniforms.h>
#include <three/renderers/gl/GLProgram.h>
#include <three/scenes/Fog.h>
#include <optional>
namespace three {
	namespace gl {

		struct Properties
		{
			GLuint image_textureCube = 0;
			bool glInit = false;
			GLuint texture=0;
			float currentAnisotropy=0;
			GLint version = -1;
			int maxMipLevel = 0;
			GLuint framebuffer=0;
			GLuint depthbuffer=0;
			std::vector<GLint> framebuffers;
			std::vector<GLint> depthbuffers;
			GLint multisampledFramebuffer=0;
			GLint colorRenderbuffer=0;
			GLint depthRenderbuffer=0;
		};

		struct MaterialProperties {
			int version=-1;
			GLProgram::ptr program;
			int lightsStateVersion;
			Fog::ptr fog;
			std::vector<float> clippingState;
			bool needsLights = false;
			bool receiveShadow = false;
			UniformValues uniforms;
			LightsHash lightsHash;
			size_t numClippingPlanes = 0;
			size_t numIntersection = 0;
			std::string shaderID;
			GLShader shader;
			TextureEncoding outputEncoding;
			std::vector<GLUniform::ptr> uniformsList;
			Texture::ptr envMap;
			Texture::ptr environment;
		};

		class GLProperties {
			std::unordered_map<sole::uuid, Properties> properties;
			std::unordered_map<sole::uuid, MaterialProperties> materialProperties;

		public :
			
			using ptr = std::shared_ptr<GLProperties>;

			GLProperties() {}
			virtual ~GLProperties() = default;
			
			Properties& getProperties(const sole::uuid& uuid) {
				return properties[uuid];
			}
			MaterialProperties& getMaterialProperties(const sole::uuid& uuid) {
				return materialProperties[uuid];
			}
			void remove(const sole::uuid& uuid) {
				properties.erase(uuid);
			}
			//template<typename T>
			//void update(const std::string& objectKey, const std::string& key, const T& val) {
			//

			//	if (contains(key)) {

			//		/*std::shared_ptr<T> temp = std::any_cast<std::shared_ptr<T>>(table->at(key));
			//		
			//		delete temp.get();
			//		
			//		temp.reset();*/
			//		remove(key);
			//	}

			//	//table->insert({ key,std::shared_ptr(new T(val)) });
			//	add(val);

			//}		
			
		};
			
	}
}
#endif