#ifndef GLPROGRAMS_THREE_H
#define GLPROGRAMS_THREE_H
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <three/textures/Texture.h>
#include <three/renderers/gl/GLProgram.h>
#include <three/renderers/gl/GLExtensions.h>
#include <three/renderers/gl/GLCapabilities.h>
#include <three/lights/Light.h>
#include <three/renderers/gl/GLBindingStates.h>
#include <three/renderers/gl/GLClipping.h>
namespace three {	
	class Material;
	class Light;
	class GLLights;
	class Scene;
	class Object3D;
	class SkinnedMesh;
	class GLRenderer;
	namespace gl {
		class GLCubeMap;

		class GLPrograms {
		private:			
			int getMaxBones(const SkinnedMesh& skinnedMesh);
		protected:
			

			bool isGL2 = false;
			
			bool logarithmicDepthBuffer = false;
			
			bool floatVertexTextures = false;

			std::string precision;

			int maxVertexUniforms = 0;

			bool vertexTextures = false;

			
			/*std::unordered_map<size_t, std::string> shaderIDs = {
				{getStringHashcode("MeshDepthMaterial"), "depth"},
				{getStringHashcode("MeshDistanceMaterial"), "distanceRGBA"},
				{getStringHashcode("MeshNormalMaterial"), "normal"},
				{getStringHashcode("MeshBasicMaterial"), "basic"},
				{getStringHashcode("MeshLambertMaterial"), "lambert"},
				{getStringHashcode("MeshPhongMaterial"), "phong"},
				{getStringHashcode("MeshToonMaterial"), "toon"},
				{getStringHashcode("MeshStandardMaterial"), "physical"},
				{getStringHashcode("MeshPhysicalMaterial"), "physical"},
				{getStringHashcode("MeshMatcapMaterial"), "matcap"},
				{getStringHashcode("LineBasicMaterial"), "basic"},
				{getStringHashcode("LineDashedMaterial"), "dashed"},
				{getStringHashcode("PointsMaterial"), "points"},
				{getStringHashcode("ShadowMaterial"), "shadow"},
				{getStringHashcode("SpriteMaterial"), "sprite"}
			};*/

			std::vector<std::string> parameterNames = {
				"precision","isGL2","supportsVertexTextures",	"outputEncoding","instancing","instancingColor",
				"map","mapEncoding","matcap","matcapEncoding","envMap","envMapMode",	"envMapEncoding","envMapCubeUV",
				"lightMap","lightMapEncoding","aoMap","emissiveMap","emissiveMapEncoding","bumpMap","normalMap","objectSpaceNormalMap","tangentSpaceNormalMap","clearcoatMap","clearcoatNormalMap","clearcoatNormalMap","displacementMap","specularMap",
				"roughnessMap","metalnessMap","gradientMap",
				"alphaMap","combine","vertexColors",	"vertexTangents","vertexUvs","uvsVertexOnly","fog","useFog","fogExp2",
				"flatShading","sizeAttenuation","logarithmicDepthBuffer","skinning",
				"maxBones","useVertexTexture","morphTargets",	"morphNormals",
				"maxMorphTargets",	"maxMorphNormals",	"premultipliedAlpha",
				"numDirLights","numPointLights","numSpotLights","numHemiLights","numRectAreaLights","numDirLightShadows",	"numPointLightShadows","numSpotLightShadows",
				"shadowMapEnabled","shadowMapType","toneMapping","physicallyCorrectLights",
				"alphaTest","doubleSided",	"flipSided","numClippingPlanes","numClipIntersection","depthPacking","dithering",
				"sheen","transmissionMap"
			};

			GLExtensions::ptr extensions;

			GLCapabilities::ptr capabilities;

			GLCubeMap& cubeMaps;

			GLBindingStates::ptr bindingStates;

			GLClipping& clipping;

		public :

			std::vector<GLProgram::ptr> programs;

			using ptr = std::shared_ptr<GLPrograms>;


			GLPrograms(GLCubeMap& cubeMaps,const GLExtensions::ptr& extensions,const GLCapabilities::ptr& capabilities,const GLBindingStates::ptr& bindingStates,GLClipping& clipping);
						
			virtual ~GLPrograms() = default;

			std::shared_ptr<ProgramParameters> getParameters(three::GLRenderer& renderer,const Material::ptr& material, GLLights& lights, const std::vector<Light::ptr>& shadows,const Scene* scene, Object3D& object);

			std::string getProgramCacheKey(const Material& material, const ProgramParameters& parameters);

			GLProgram::ptr acquireProgram(three::GLRenderer& renderer,const ProgramParameters& parameters, const std::string& code);

			void releaseProgram(GLProgram::ptr& program);

			UniformValues getUniforms(const Material::ptr& material);

		};
	}
}
#endif