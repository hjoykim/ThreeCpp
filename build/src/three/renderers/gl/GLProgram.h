#ifndef GLPROGRAM_THREE_H
#define GLPROGRAM_THREE_H
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <three/Constants.h>
#include <three/renderers/gl/GLShader.h>
#include <three/renderers/gl/GLUniforms.h>
#include <three/utils/simplesignal.h>
#include <regex>
#include <three/utils/sole.h>
#include <three/Types.h>
#include <three/materials/Material.h>
#include <three/renderers/gl/GLExtensions.h>
namespace three {
	
	class Material;

	class GLRenderer;

	namespace gl {		
		
		/*else {
			   array.push_back(parameters.get<std::string>("fragmentShader"));
			   array.push_back(parameters.get<std::string>("vertexShader"));
		   }*/
		struct ProgramParameters {
			using ptr = std::shared_ptr<ProgramParameters>;
			std::string name = "";
			bool isGL2 = false;
			std::string shaderID;
			std::string shaderName="";
			std::string precision="highp";
			std::string fragmentShader="";
			std::string vertexShader="";
			std::unordered_map<std::string, std::string> defines;
			std::unordered_map<std::string, float> definesFloat;
			bool isRawShaderMaterial = false;
			bool instancingColor = false;
			bool supportsVertexTextures = false;
			TextureEncoding outputEncoding = TextureEncoding::Unknown;
			bool instancing = false;
			bool map = false;
			TextureEncoding mapEncoding = TextureEncoding::Unknown;
			bool matcap = false;
			TextureEncoding matcapEncoding = TextureEncoding::Unknown;
			bool envMap = false;
			TextureMapping envMapMode = TextureMapping::Unknown;
			TextureEncoding envMapEncoding = TextureEncoding::Unknown;
			bool envMapCubeUV = false;
			bool lightMap = false;
			TextureEncoding lightMapEncoding = TextureEncoding::Unknown;
			bool aoMap = false;
			bool emissiveMap = false;
			TextureEncoding emissiveMapEncoding = TextureEncoding::Unknown;
			bool bumpMap = false;
			bool normalMap = false;
			bool clearcoatMap = false;
			bool clearcoatRoughnessMap = false;
			bool objectSpaceNormalMap = false;
			bool tangentSpaceNormalMap = false;
			bool clearcoatNormalMap = false;
			bool displacementMap = false;
			bool specularMap = false;
			bool roughnessMap = false;
			bool metalnessMap = false;
			bool gradientMap = false;
			bool alphaMap = false;
			Combine combine = Combine::None;
			bool vertexColors = false;
			bool vertexTangents = false;
			bool vertexUvs = false;
			bool uvsVertexOnly = false;
			bool fog = false;
			bool useFog = false;
			bool fogExp2 = false;
			bool flatShading = false;
			bool sizeAttenuation = false;
			bool logarithmicDepthBuffer = false;
			bool skinning = false;
			int maxBones = 0;
			bool useVertexTexture = false;
			bool morphTargets = false;
			bool morphNormals = false;
			int maxMorphTargets =0;
			int maxMorphNormals = 0;;
			bool premultipliedAlpha = false;
			int numDirLights = 0;
			int numPointLights = 0;
			int numSpotLights = 0;
			int numHemiLights = 0;
			int numRectAreaLights = 0;
			int numDirLightShadows = 0;
			int numPointLightShadows =0;
			int numSpotLightShadows =0;
			bool shadowMapEnabled = false;
			ShadowMapType shadowMapType = ShadowMapType::NoneShadowMap;
			ToneMapping toneMapping = ToneMapping::NoToneMapping;
			bool physicallyCorrectLights = false;
			float alphaTest = false;
			Side side = Side::None;
			bool doubleSided = false;
			bool flipSided = false;
			int numClippingPlanes = 0;
			int numClipIntersection = 0;
			DepthPackingStrategies depthPacking = DepthPackingStrategies::None;
			bool dithering = false;
			bool sheen = false;
			bool transmissionMap = false;
			bool extensionDerivatives = false;
			bool extensionFragDepth = false;
			bool extensionDrawBuffers = false;
			bool rendererExtensionDrawBuffers = false;
			bool rendererExtensionFragDepth = false;
			bool shaderTextureLOD = false;
			bool extensionShaderTextureLOD = false;
			bool renderExtensionShaderTextureLOD = false;
			GLSLVersion glslVersion;
			std::string indexOfAttributeName;
			std::string customProgramCacheKey;
			UniformValues uniforms;
		};

			
		
		class GLBindingStates;

		class GLProgram {
			static unsigned programId;
			struct ProgramDiagnostics {
				bool runnable;
				Material material;
				std::string programLog;
			};
		private:
			

			std::vector<std::string> getEncodingComponents(TextureEncoding encoding);

			std::string getShaderErrors(const GLShader& shader, const std::string& type);
			
			std::string getTexelDecodingFunction(const std::string& functionName, TextureEncoding encoding);

			std::string getTexelEncodingFunction(const std::string& functionName, TextureEncoding encoding);

			std::string getToneMappingFunction(const std::string& functionName, ToneMapping toneMapping);

			std::string generateExtensions(const ProgramParameters& parameters);

			std::string generateDefines(const std::unordered_map<std::string, std::string>& defines);

			void fetchAttributeLocation(const int program, std::unordered_map<std::string, GLint>& attribute);

			std::string& replaceLightsNums(std::string& str, const ProgramParameters& parameters);

			std::string replaceClippingPlaneNums(std::string& str, const ProgramParameters& parameters);			

			std::string resolveIncludesRX(std::string& source);

			std::string includeReplacerRX(std::string& match);

			std::string resolveIncludes(std::string& source);

			std::string includeReplacer(std::smatch& match);

			std::string unrollLoops(const std::string& source);

			std::string resolveIncludeUX(const std::string& source);

			std::string unrollLoopsUX(const std::string& source);

			std::string loopReplacer(std::smatch& match);

			std::string generatePrecision(const ProgramParameters& parameters);

			std::string generateShadowMapTypeDefine(const ProgramParameters& parameters);

			std::string generateEnvMapTypeDefine(const ProgramParameters& parameters);

			std::string generateEnvMapModeDefine(const ProgramParameters& parameters);

			std::string generateEnvMapBlendingDefine(const ProgramParameters& parameters);			

			std::string addLineNumbers(const std::string& code);		

		public:
			using ptr = std::shared_ptr<GLProgram>;
			
			std::string name;

			std::string cacheKey;

			unsigned usedTimes = 1;

			int id;

			sole::uuid uuid = sole::uuid1();

			GLuint program = 0;

			GLShader vertexShader;

			GLShader fragmentShader;

			GLUniforms::ptr cachedUniforms;

			std::unordered_map<std::string, GLint> cachedAttributes;

			ProgramDiagnostics diagnostics;

			three::GLRenderer& renderer;

			std::shared_ptr<GLBindingStates> bindingStates;
			//GLProgram();

			GLProgram(GLRenderer& renderer,const GLExtensions::ptr& extensions, const std::string& cacheKey, const ProgramParameters& parameters,const std::shared_ptr<GLBindingStates>& bindingStates);

			~GLProgram() = default;
			/*{
				if (this->program) {
					glDeleteProgram(this->program);
					this->program = 0;
				}
			}	*/
			GLUniforms::ptr getUniforms();

			std::unordered_map<std::string, GLint>& getAttributes();
			
		};
	}
}
#endif