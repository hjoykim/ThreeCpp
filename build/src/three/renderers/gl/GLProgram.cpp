#include "GLProgram.h"
#include <three/lights/Light.h>
#include <three/renderers/GLRenderer.h>
#include <three/renderers/shaders/ShaderChunk.h>
#include <three/renderers/gl/GLCapabilities.h>
#include <three/materials/RawShaderMaterial.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <pcrecpp.h>
#include <three/utils/pcrscpp.h>
#include <three/renderers/gl/GLBindingStates.h>

typedef std::shared_ptr<bool> boolptr;
typedef std::shared_ptr<int> intptr;
typedef std::shared_ptr<float> floatptr;
typedef std::shared_ptr<std::string> stringptr;

namespace three {
	namespace gl {
		unsigned GLProgram::programId = 0;
		void sendDebugMessage(const std::wstring& name) {
			std::wcout << name << std::endl;
		}
		
		using namespace std;

		pcrscpp::replace pcrscpp_rs;

		GLProgram::GLProgram(GLRenderer& renderer, const GLExtensions::ptr& extensions, const std::string& cacheKey, const ProgramParameters& parameters, const std::shared_ptr<GLBindingStates>& bindingStates)
			:renderer(renderer), bindingStates(bindingStates)
		{
			id = GLProgram::programId++;			

			this->cacheKey = cacheKey;

			std::unordered_map<std::string,std::string> definesString = parameters.defines;

			std::string vShader = parameters.vertexShader;
			
			std::string fShader = parameters.fragmentShader;
			
			std::string shadowMapTypeDefine = generateShadowMapTypeDefine(parameters);
			
			std::string envMapTypeDefine = generateEnvMapTypeDefine(parameters);

			std::string envMapModeDefine = generateEnvMapModeDefine(parameters);

			std::string envMapBlendingDefine = generateEnvMapBlendingDefine(parameters);

			float gammaFactorDefine = (renderer.gammaFactor > 0) ? renderer.gammaFactor : 1.0f;

			std::string customExtensions = parameters.isGL2 ? "" : generateExtensions(parameters);
			
			std::string customDefines = generateDefines(definesString);

			//std::string customDefinesFloat = generateDefines(material.definesFloat);

			this->program = glCreateProgram();

			stringstream prefixVertex, prefixFragment;

			GLSLVersion glslVersion = parameters.glslVersion;


			if (parameters.isRawShaderMaterial) {
				prefixVertex << customDefines << endl;
				if (customDefines.size() > 0)
					prefixVertex << endl;

				prefixFragment << customExtensions << customDefines << endl;
				if (!customExtensions.empty() || !customDefines.empty())
					prefixFragment << endl;
			}
			else {
#ifdef OPENGL_ES_3_2
				prefixVertex << "#version 300 es" << endl;
				
#else
				prefixVertex << "#version 440" << endl;
				prefixVertex << generatePrecision(parameters) << endl;
				//prefixVertex << "#version 140" << endl;
#endif
				prefixVertex << "#define attribute in" << endl;
				prefixVertex << "#define varying out" << endl;
				prefixVertex << "#define texture2D texture" << endl;

				prefixVertex << "#define SHADER_NAME " << parameters.shaderName << endl;

				prefixVertex << customDefines;

				if (parameters.instancing) prefixVertex << "#define USE_INSTANCING" << endl;
				if(parameters.instancingColor) prefixVertex << "#define USE_INSTANCING_COLOR" << endl;
				if (parameters.supportsVertexTextures) prefixVertex << "#define VERTEX_TEXTURES" << endl;

				prefixVertex << "#define GAMMA_FACTOR " << std::to_string(gammaFactorDefine) << endl;

				prefixVertex << "#define MAX_BONES " << parameters.maxBones << endl;

				if (parameters.useFog && parameters.fog) prefixVertex << "#define USE_FOG" << endl;
				if (parameters.useFog && parameters.fogExp2) prefixVertex << "#define FOG_EXP2" << endl;

				if (parameters.map)  prefixVertex << "#define USE_MAP" << endl;
				if (parameters.envMap) prefixVertex << "#define USE_ENVMAP" << endl;
				if (parameters.envMap) prefixVertex << "#define " + envMapModeDefine << endl;
				if (parameters.lightMap) prefixVertex << "#define USE_LIGHTMAP" << endl;
				if (parameters.aoMap) prefixVertex << "#define USE_AOMAP" << endl;
				if (parameters.emissiveMap) prefixVertex << "#define USE_EMISSIVEMAP" << endl;
				if (parameters.bumpMap) prefixVertex << "#define USE_BUMPMAP" << endl;
				if (parameters.normalMap) prefixVertex << "#define USE_NORMALMAP" << endl;
				if (parameters.normalMap && parameters.objectSpaceNormalMap) prefixVertex << "#define OBJECTSPACE_NORMALMAP" << endl;
				if (parameters.normalMap && parameters.tangentSpaceNormalMap) prefixVertex << "#define TANGENTSPACE_NORMALMAP" << endl;

				if (parameters.clearcoatMap) prefixVertex << "#define USE_CLEARCOATMAP" << endl;
				if (parameters.clearcoatRoughnessMap) prefixVertex << "#define USE_CLEARCOAT_ROUGHNESSMAP" << endl;
				if (parameters.clearcoatNormalMap) prefixVertex << "#define USE_CLEARCOAT_NORMALMAP" << endl;
				if (parameters.displacementMap && parameters.supportsVertexTextures) prefixVertex << "#define USE_DISPLACEMENTMAP" << endl;
				if (parameters.specularMap) prefixVertex << "#define USE_SPECULARMAP" << endl;
				if (parameters.roughnessMap) prefixVertex << "#define USE_ROUGHNESSMAP" << endl;
				if (parameters.metalnessMap) prefixVertex << "#define USE_METALNESSMAP" << endl;
				if (parameters.alphaMap) prefixVertex << "#define USE_ALPHAMAP" << endl;
				if (parameters.transmissionMap) prefixVertex << "#define USE_TRANSMISSIONMAP" << endl;

				if (parameters.vertexTangents) prefixVertex << "#define USE_TANGENT" << endl;
				if (parameters.vertexColors) prefixVertex << "#define USE_COLOR" << endl;
				if (parameters.vertexUvs) prefixVertex << "#define USE_UV" << endl;
				if (parameters.uvsVertexOnly) prefixVertex << "#define UVS_VERTEX_ONLY" << endl;

				if (parameters.flatShading) prefixVertex << "#define FLAT_SHADED" << endl;

				if (parameters.skinning) prefixVertex << "#define USE_SKINNING" << endl;
				if (parameters.useVertexTexture) prefixVertex << "#define BONE_TEXTURE" << endl;

				if (parameters.morphTargets) prefixVertex << "#define USE_MORPHTARGETS" << endl;
				if (parameters.morphNormals && parameters.flatShading == false) prefixVertex << "#define USE_MORPHNORMALS" << endl;
				if (parameters.doubleSided) prefixVertex << "#define DOUBLE_SIDED" << endl;
				if (parameters.flipSided) prefixVertex << "#define FLIP_SIDED" << endl;

				if (parameters.shadowMapEnabled) prefixVertex << "#define USE_SHADOWMAP" << endl;
				if (parameters.shadowMapEnabled) prefixVertex << "#define " + shadowMapTypeDefine << endl;

				if (parameters.sizeAttenuation) prefixVertex << "#define USE_SIZEATTENUATION" << endl;

				if (parameters.logarithmicDepthBuffer) prefixVertex << "#define USE_LOGDEPTHBUF" << endl;
				if (parameters.logarithmicDepthBuffer && parameters.rendererExtensionFragDepth) prefixVertex << "#define USE_LOGDEPTHBUF_EXT" << endl;

				prefixVertex << "uniform mat4 modelMatrix;" << endl;
				prefixVertex << "uniform mat4 modelViewMatrix;" << endl;
				prefixVertex << "uniform mat4 projectionMatrix;" << endl;
				prefixVertex << "uniform mat4 viewMatrix;" << endl;
				prefixVertex << "uniform mat3 normalMatrix;" << endl;
				prefixVertex << "uniform vec3 cameraPosition;" << endl;
				prefixVertex << "uniform bool isOrthographic;" << endl;

				prefixVertex << "#ifdef USE_INSTANCING" << endl;

				prefixVertex << " attribute mat4 instanceMatrix;" << endl;

				prefixVertex << "#endif" << endl;

				prefixVertex << "#ifdef USE_INSTANCING_COLOR" << endl;

				prefixVertex << " attribute vec3 instanceColor;" << endl;

				prefixVertex << "#endif" << endl;


				prefixVertex << "attribute vec3 position;" << endl;
				prefixVertex << "attribute vec3 normal;" << endl;
				prefixVertex << "attribute vec2 uv;" << endl;

				prefixVertex << "#ifdef USE_TANGENT" << endl;

				prefixVertex << "	attribute vec4 tangent;" << endl;

				prefixVertex << "#endif" << endl;

				prefixVertex << "#ifdef USE_COLOR" << endl;

				prefixVertex << "	attribute vec3 color;" << endl;

				prefixVertex << "#endif" << endl;

				prefixVertex << "#ifdef USE_MORPHTARGETS" << endl;

				prefixVertex << "	attribute vec3 morphTarget0;" << endl;
				prefixVertex << "	attribute vec3 morphTarget1;" << endl;
				prefixVertex << "	attribute vec3 morphTarget2;" << endl;
				prefixVertex << "	attribute vec3 morphTarget3;" << endl;

				prefixVertex << "	#ifdef USE_MORPHNORMALS" << endl;

				prefixVertex << "		attribute vec3 morphNormal0;" << endl;
				prefixVertex << "		attribute vec3 morphNormal1;" << endl;
				prefixVertex << "		attribute vec3 morphNormal2;" << endl;
				prefixVertex << "		attribute vec3 morphNormal3;" << endl;

				prefixVertex << "	#else" << endl;

				prefixVertex << "		attribute vec3 morphTarget4;" << endl;
				prefixVertex << "		attribute vec3 morphTarget5;" << endl;
				prefixVertex << "		attribute vec3 morphTarget6;" << endl;
				prefixVertex << "		attribute vec3 morphTarget7;" << endl;

				prefixVertex << "	#endif" << endl;

				prefixVertex << "#endif" << endl;

				prefixVertex << "#ifdef USE_SKINNING" << endl;

				prefixVertex << "	attribute vec4 skinIndex;" << endl;
				prefixVertex << "	attribute vec4 skinWeight;" << endl;

				prefixVertex << "#endif" << endl;


#ifdef OPENGL_ES_3_2
				prefixFragment << "#version 300 es" << endl;
				
#else
				prefixFragment << "#version 440" << endl;
				prefixFragment << generatePrecision(parameters) << endl;
				//prefixFragment << "#version 140" << endl;
#endif
				prefixFragment << customExtensions << endl;
				prefixFragment << "#define varying in" << endl;
				prefixFragment << "out highp vec4 pc_fragColor;" << endl;
				prefixFragment << "#define gl_FragColor pc_fragColor" << endl;
				prefixFragment << "#define gl_FragDepthEXT gl_FragDepth" << endl;
				prefixFragment << "#define texture2D texture" << endl;
				prefixFragment << "#define textureCube texture" << endl;
				prefixFragment << "#define texture2DProj textureProj" << endl;
				prefixFragment << "#define texture2DLodEXT textureLod" << endl;
				prefixFragment << "#define texture2DProjLodEXT textureProjLod" << endl;
				prefixFragment << "#define textureCubeLodEXT textureLod" << endl;
				prefixFragment << "#define texture2DGradEXT textureGrad" << endl;
				prefixFragment << "#define texture2DProjGradEXT textureProjGrad" << endl;
				prefixFragment << "#define textureCubeGradEXT textureGrad" << endl;


				prefixFragment << "#define SHADER_NAME " + parameters.shaderName << endl;

				prefixFragment << customDefines;

				float alphaTest = parameters.alphaTest;
				if (alphaTest) {
					prefixFragment << "#define ALPHATEST " + std::to_string(alphaTest);
					if (std::fmod(alphaTest, 1.0f) > 0)
						prefixFragment << "";
					else
						prefixFragment << ".0" << endl; // add ".0" if integer
				}
				prefixFragment << "#define GAMMA_FACTOR " << gammaFactorDefine << endl;

				if (parameters.useFog && parameters.fog)  prefixFragment << "#define USE_FOG" << endl;
				if (parameters.useFog && parameters.fogExp2)  prefixFragment << "#define FOG_EXP2" << endl;

				if (parameters.map)  prefixFragment << "#define USE_MAP" << endl;
				if (parameters.matcap)  prefixFragment << "#define USE_MATCAP" << endl;
				if (parameters.envMap)  prefixFragment << "#define USE_ENVMAP" << endl;
				if (parameters.envMap)  prefixFragment << "#define " + envMapTypeDefine << endl;
				if (parameters.envMap)  prefixFragment << "#define " + envMapModeDefine << endl;
				if (parameters.envMap)  prefixFragment << "#define " + envMapBlendingDefine << endl;
				if (parameters.lightMap)  prefixFragment << "#define USE_LIGHTMAP" << endl;
				if (parameters.aoMap)  prefixFragment << "#define USE_AOMAP" << endl;
				if (parameters.emissiveMap)  prefixFragment << "#define USE_EMISSIVEMAP" << endl;
				if (parameters.bumpMap)  prefixFragment << "#define USE_BUMPMAP" << endl;
				if (parameters.normalMap)  prefixFragment << "#define USE_NORMALMAP" << endl;
				if (parameters.normalMap && parameters.objectSpaceNormalMap)  prefixFragment << "#define OBJECTSPACE_NORMALMAP" << endl;
				if (parameters.normalMap && parameters.tangentSpaceNormalMap)  prefixFragment << "#define TANGENTSPACE_NORMALMAP" << endl;
				if (parameters.clearcoatMap)  prefixFragment << "#define USE_CLEARCOATMAP" << endl;
				if (parameters.clearcoatRoughnessMap)  prefixFragment << "#define USE_CLEARCOAT_ROUGHNESSMAP" << endl;
				if (parameters.clearcoatNormalMap)  prefixFragment << "#define USE_CLEARCOAT_NORMALMAP" << endl;
				if (parameters.specularMap)  prefixFragment << "#define USE_SPECULARMAP" << endl;
				if (parameters.roughnessMap)  prefixFragment << "#define USE_ROUGHNESSMAP" << endl;
				if (parameters.metalnessMap)  prefixFragment << "#define USE_METALNESSMAP" << endl;
				if (parameters.alphaMap)  prefixFragment << "#define USE_ALPHAMAP" << endl;

				if (parameters.sheen)  prefixFragment << "#define USE_SHEEN" << endl;
				if (parameters.transmissionMap) prefixFragment << "#define USE_TRANSMISSIONMAP" << endl;

				if (parameters.vertexTangents)  prefixFragment << "#define USE_TANGENT" << endl;
				if (parameters.vertexColors || parameters.instancingColor)  prefixFragment << "#define USE_COLOR" << endl;
				if (parameters.vertexUvs)  prefixFragment << "#define USE_UV" << endl;
				if (parameters.uvsVertexOnly)  prefixFragment << "#define UVS_VERTEX_ONLY" << endl;

				if (parameters.gradientMap)  prefixFragment << "#define USE_GRADIENTMAP" << endl;

				if (parameters.flatShading)  prefixFragment << "#define FLAT_SHADED" << endl;

				if (parameters.doubleSided)  prefixFragment << "#define DOUBLE_SIDED" << endl;
				if (parameters.flipSided)  prefixFragment << "#define FLIP_SIDED" << endl;

				if (parameters.shadowMapEnabled)  prefixFragment << "#define USE_SHADOWMAP" << endl;
				if (parameters.shadowMapEnabled)  prefixFragment << "#define " + shadowMapTypeDefine << endl;

				if (parameters.premultipliedAlpha)  prefixFragment << "#define PREMULTIPLIED_ALPHA" << endl;

				if (parameters.physicallyCorrectLights)  prefixFragment << "#define PHYSICALLY_CORRECT_LIGHTS" << endl;

				if (parameters.logarithmicDepthBuffer)  prefixFragment << "#define USE_LOGDEPTHBUF" << endl;
				if (parameters.logarithmicDepthBuffer) {								
						if (parameters.rendererExtensionFragDepth)
							prefixFragment << "#define USE_LOGDEPTHBUF_EXT" << endl;
				}

				bool extensionShaderTextureLOD = parameters.extensionShaderTextureLOD;
				

				bool rendererExtensionShaderTextureLod = parameters.renderExtensionShaderTextureLOD;

				if ((extensionShaderTextureLOD || parameters.envMap) && rendererExtensionShaderTextureLod)
					prefixFragment << "#define TEXTURE_LOD_EXT" << endl;

				prefixFragment << "uniform mat4 viewMatrix;" << endl;
				prefixFragment << "uniform vec3 cameraPosition;" << endl;
				prefixFragment << "uniform bool isOrthographic;" << endl;

				if (parameters.toneMapping != ToneMapping::NoToneMapping)  prefixFragment << "#define TONE_MAPPING" << endl;
				if (parameters.toneMapping != ToneMapping::NoToneMapping) prefixFragment << getShaderChunk(ShaderLibID::tonemapping_pars_fragment) << endl; // this code is required here because it is used by the toneMapping() function defined below
				if (parameters.toneMapping != ToneMapping::NoToneMapping) prefixFragment << getToneMappingFunction("toneMapping", parameters.toneMapping) << endl;

				if (parameters.dithering)  prefixFragment << "#define DITHERING" << endl;

				if (parameters.outputEncoding!=TextureEncoding::Unknown ||
					parameters.mapEncoding != TextureEncoding::Unknown ||
					parameters.matcapEncoding != TextureEncoding::Unknown ||
					parameters.envMapEncoding != TextureEncoding::Unknown ||
					parameters.emissiveMapEncoding != TextureEncoding::Unknown ||
					parameters.lightMapEncoding != TextureEncoding::Unknown)
					prefixFragment << getShaderChunk(ShaderLibID::encodings_pars_fragment) << endl; // this code is required here because it is used by the various encoding/decoding function defined below
				if (parameters.mapEncoding!=TextureEncoding::Unknown) prefixFragment << getTexelDecodingFunction("mapTexelToLinear", parameters.mapEncoding) << endl;
				if (parameters.matcapEncoding != TextureEncoding::Unknown) prefixFragment << getTexelDecodingFunction("matcapTexelToLinear", parameters.matcapEncoding) << endl;
				if (parameters.envMapEncoding != TextureEncoding::Unknown) prefixFragment << getTexelDecodingFunction("envMapTexelToLinear", parameters.envMapEncoding) << endl;
				if (parameters.emissiveMapEncoding != TextureEncoding::Unknown) prefixFragment << getTexelDecodingFunction("emissiveMapTexelToLinear", parameters.emissiveMapEncoding) << endl;
				if (parameters.lightMapEncoding != TextureEncoding::Unknown) prefixFragment << getTexelDecodingFunction("lightMapTexelToLinear", parameters.lightMapEncoding) << endl;
				if (parameters.outputEncoding != TextureEncoding::Unknown) prefixFragment << getTexelEncodingFunction("linearToOutputTexel", parameters.outputEncoding) << endl;

				if (parameters.depthPacking!=DepthPackingStrategies::None)  prefixFragment << "#define DEPTH_PACKING " + std::to_string((int)parameters.depthPacking) << endl;
			}
			pcrscpp_rs.remove_jobs();
			vShader = resolveIncludesRX(vShader);
			vShader = replaceLightsNums(vShader, parameters);
			vShader = replaceClippingPlaneNums(vShader, parameters);

			pcrscpp_rs.remove_jobs();
			fShader = resolveIncludesRX(fShader);
			fShader = replaceLightsNums(fShader, parameters);
			fShader = replaceClippingPlaneNums(fShader, parameters);

			vShader = unrollLoops(vShader);
			fShader = unrollLoops(fShader);
			
			

			if (parameters.isGL2 && parameters.isRawShaderMaterial) {
				bool isGLSL3ShaderMaterial = false;				

				// GLSL 3.0 conversion
				
				stringstream prefixVertex3;

#ifdef OPENGL_ES_3_2
				prefixVertex3 << "#version 300 es" << endl
							 << generatePrecision(parameters) << endl
#else
				prefixVertex3 << "#version 440" << endl
					<<" precision highp float;" <<endl
					<< "#define HIGH_PRECISION" << endl
#endif
					          << "#define attribute in" << endl
							  << "#define varying out" << endl
							  << "#define texture2D texture" << endl
							  << prefixVertex.str() <<endl;
				prefixVertex.swap(prefixVertex3);

					
				stringstream prefixFragment3;
				prefixFragment3
#ifdef OPENGL_ES_3_2
					<< "#version 300 es" << endl
					<< generatePrecision(parameters) << endl
#else
					<< "#version 440" << endl	
					<<"precision highp float;"<<endl
					<<"#define HIGH_PRECISION"<<endl
#endif
					<< "#define varying in" << endl
					<< "out highp vec4 pc_fragColor;" << endl
					<< "#define gl_FragColor pc_fragColor" << endl
					<< "#define gl_FragDepthEXT gl_FragDepth" << endl
					<< "#define texture2D texture" << endl
					<< "#define textureCube texture" << endl
					<< "#define texture2DProj textureProj" << endl
					<< "#define texture2DLodEXT textureLod" << endl
					<< "#define texture2DProjLodEXT textureProjLod" << endl
					<< "#define textureCubeLodEXT textureLod" << endl
					<< "#define texture2DGradEXT textureGrad" << endl
					<< "#define texture2DProjGradEXT textureProjGrad" << endl
					<< "#define textureCubeGradEXT textureGrad" << endl;
				prefixFragment.swap(prefixFragment3);					
			}
			

			prefixVertex << vShader << endl;
			prefixFragment << fShader << endl;

			std::string vertexGlsl = prefixVertex.str();
			std::string fragmentGlsl = prefixFragment.str();

			// for glsl Debug
			//std::ofstream vshaderfile;
			//vshaderfile.open(parameters.shaderName+"_vshader.txt", std::ios_base::out);
			//vshaderfile << vertexGlsl;
			//vshaderfile.close();

			//std::ofstream fshaderfile;
			//fshaderfile.open(parameters.shaderName+"_fshader.txt", std::ios_base::out);
			//fshaderfile << fragmentGlsl;
			//fshaderfile.close();

			GLShader vertexShader = GLShader(GL_VERTEX_SHADER, vertexGlsl);
			GLShader fragmentShader = GLShader(GL_FRAGMENT_SHADER, fragmentGlsl);

			glAttachShader(this->program, vertexShader.shader);
			glAttachShader(this->program, fragmentShader.shader);

			if (!parameters.indexOfAttributeName.empty()) {
				glBindAttribLocation(this->program, 0, parameters.indexOfAttributeName.c_str());
			}
			else if (parameters.morphTargets) {
				glBindAttribLocation(program, 0, "position");
			}

			glLinkProgram(this->program);
			bool checkShaderErrors = true;// renderer.debug.count("checkShaderErrors") > 0;
			//if (checkShaderErrors)
			//	checkShaderErrors = renderer.debug["checkShaderErrors"];
			if (checkShaderErrors) {
				char programLog[1024];
				glGetProgramInfoLog(program, 1024, 0, programLog);
				
				char vertexLog[1024];
				glGetShaderInfoLog(vertexShader.shader, 1024, 0, vertexLog);
				
				char fragmentLog[1024];
				glGetShaderInfoLog(fragmentShader.shader, 1024, 0, fragmentLog);

				int linkStatus;
				bool runnable = true;
				bool haveDiagnostics = true;
				glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
				if (linkStatus == 0) {
					runnable = false;
					string vertexError = getShaderErrors(vertexShader, "vertex");
					if (!vertexError.empty()) {
					#ifdef _DEBUG
						std::ofstream outfile;						
						outfile.open("vertexError.txt", std::ios_base::out);
						outfile << vertexError;
						outfile.close();
						//sendDebugMessage(std::wstring().assign(vertexError.begin(), vertexError.end()));
					#endif
					}
					string fragmentErrors = getShaderErrors(fragmentShader, "fragment");
					if (!fragmentErrors.empty()) {
					#ifdef _DEBUG
						std::ofstream outfile;
						outfile.open("fragmentError.txt", std::ios_base::out);
						outfile << fragmentErrors;
						outfile.close();
						//sendDebugMessage(std::wstring().assign(fragmentErrors.begin(), fragmentErrors.end()));
					#endif
					}
					int validateState;
					glGetProgramiv(program, GL_VALIDATE_STATUS, &validateState);

					stringstream errorMessage;
					errorMessage << "THREE.Renderers.gl.GLProgram:Shader error " << glGetError() << " , VALIDATE_STATUS " << validateState << ", GL.GetProgramInfoLog " << programLog << endl << vertexLog << endl << fragmentLog;
#ifdef _DEBUG
					std::ofstream errorfile;
					errorfile.open("programError.txt", std::ios_base::out);
					errorfile << errorMessage.str();
					errorfile.close();
					//sendDebugMessage(std::wstring().assign(errorMessage.str().begin(), errorMessage.str().end()));
#endif
				}
				else if (!std::string().assign(programLog).empty()) {

				}
				else if (!std::string().assign(vertexLog).empty() || !std::string().assign(fragmentLog).empty()) {
					haveDiagnostics = false;
				}
				if (haveDiagnostics) {
					diagnostics.runnable= runnable;
					//diagnostics.material= material;
					diagnostics.programLog= programLog;
					//
					//
				}
			}

			glDeleteShader(vertexShader.shader);
			glDeleteShader(fragmentShader.shader);
			
		}

		std::vector<std::string> GLProgram::getEncodingComponents(TextureEncoding encoding)
		{
			std::vector<std::string> values;
			switch (encoding) {
			case TextureEncoding::LinearEncoding :
				values.push_back("Linear");values.push_back("( value )");
				return values;
			case TextureEncoding::sRGBEncoding :
				values.push_back("sRGB");values.push_back("( value )");
				return values;
			case TextureEncoding::RGBEEncoding:
				values.push_back("RGBE");values.push_back("( value )");
				return values;
			case TextureEncoding::RGBM7Encoding:
				values.push_back("RGBM");values.push_back("( value )");
				return values;
			case TextureEncoding::RGBM16Encoding:
				values.push_back("RGBM");values.push_back("( value )");
				return values;
			case TextureEncoding::RGBDEncoding:
				values.push_back("RGBD");values.push_back("( value )");
				return values;
			case TextureEncoding::GammaEncoding:
				values.push_back("Gamma");values.push_back("( value )");
				return values;
			case TextureEncoding::LogLuvEncoding:
				values.push_back("LogLuv");values.push_back("( value )");
				return values;
			default :
				values.push_back("");values.push_back("");
				return values;
			}			
		}

		std::string GLProgram::getShaderErrors(const GLShader& shader, const std::string& type)
		{
			int status;
			glGetShaderiv(shader.shader, GL_COMPILE_STATUS, &status);
			
			GLchar infoLog[1024];
			glGetShaderInfoLog(shader.shader, 1024, NULL, infoLog);

			std::string log(infoLog);
			if (status == 1 && log.empty()) return log; // empty string

			int length;
			GLchar* source = new GLchar[4096000];

			glGetShaderSource(shader.shader, 4096000, &length, source);

			stringstream ss;

			ss<< "THREE.Renderers.gl.GLProgram: glGetShaderInfoLog() "<< type<<endl << log <<endl <<addLineNumbers(string(source));

			delete source;

			return ss.str();
		}

		std::string GLProgram::getTexelDecodingFunction(const std::string& functionName, TextureEncoding encoding)
		{
			stringstream ss;

			std::vector<std::string> components = getEncodingComponents(encoding);

			ss << "vec4 " << functionName << " ( vec4 value ) { return " << components[0] << "ToLinear"<< components[1] << ";}";

			return ss.str();
		}

		std::string GLProgram::getTexelEncodingFunction(const std::string& functionName, TextureEncoding encoding)
		{
			stringstream ss;

			std::vector<std::string> components = getEncodingComponents(encoding);

			ss << "vec4 " << functionName << " ( vec4 value ) { return LinearTo" << components[0] << components[1] << ";}";

			return ss.str();
		}

		std::string GLProgram::getToneMappingFunction(const std::string& functionName, ToneMapping toneMapping)
		{
			stringstream ss;
			string toneMappingName;
			switch (toneMapping) {

			case ToneMapping::LinearToneMapping:
				toneMappingName = "Linear";
				break;

			case ToneMapping::ReinhardToneMapping:
				toneMappingName = "Reinhard";
				break;

			case ToneMapping::Uncharted2ToneMapping:
				toneMappingName = "Uncharted2";
				break;

			case ToneMapping::CineonToneMapping:
				toneMappingName = "OptimizedCineon";
				break;

			case ToneMapping::ACESFilmicToneMapping:
				toneMappingName = "ACESFilmic";
				break;

			default:
				toneMappingName="";

			}

			ss << "vec3 " + functionName + "( vec3 color ) { return " << toneMappingName << "ToneMapping( color ); }";

			return ss.str();

		}

		std::string GLProgram::generateExtensions(const ProgramParameters& parameters)
		{
			stringstream chunks;			

			if (parameters.extensionDerivatives || parameters.envMapCubeUV || parameters.bumpMap || parameters.tangentSpaceNormalMap || parameters.clearcoatNormalMap || parameters.flatShading || parameters.shaderID=="physical")
				chunks <<  "#extension GL_OES_standard_derivatives : enable" << endl;

			if ((parameters.extensionFragDepth || parameters.logarithmicDepthBuffer) && parameters.rendererExtensionFragDepth)
				chunks << "#extension GL_EXT_frag_depth : enable"<<endl;

			if (parameters.extensionDrawBuffers && parameters.rendererExtensionDrawBuffers)
				chunks << "#extension GL_EXT_draw_buffers : require"<<endl;			

			if ((parameters.shaderTextureLOD || parameters.envMap) && parameters.renderExtensionShaderTextureLOD)
				chunks << "#extension GL_EXT_shader_texture_lod : enable" << endl;

			return chunks.str();
		}

		std::string GLProgram::generateDefines(const std::unordered_map<std::string, std::string>& defines)
		{
			if (defines.size() == 0) return {};
			stringstream ss;
			std::string strVal;
			float floatVal;
			int intVal;
			
			for (auto& entry : defines) {				
				ss << "#define " << entry.first << " " << entry.second << endl;
			}

			return ss.str();
		}

		void GLProgram::fetchAttributeLocation(const int program, std::unordered_map<std::string, GLint>& attribute)
		{
			int n;

			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &n);
			for (unsigned i = 0;i < n;i++) {
				GLsizei length;
				GLint size;
				GLenum type;
				GLchar name[101];

				glGetActiveAttrib(program, i, 100, &length, &size, &type,name);

				int location = glGetAttribLocation(program, name);

				string attributeName(name);

				attribute[attributeName] = location;
				//morphTarget
				//morphNormal				
			}			
		}

		std::string& GLProgram::replaceLightsNums(std::string& str, const ProgramParameters& parameters)
		{
			str = replace_all(str, "NUM_DIR_LIGHTS", std::to_string(parameters.numDirLights));
			str = replace_all(str, "NUM_SPOT_LIGHTS", std::to_string(parameters.numSpotLights));
			str = replace_all(str, "NUM_RECT_AREA_LIGHTS", std::to_string(parameters.numRectAreaLights));
			str = replace_all(str, "NUM_POINT_LIGHTS", std::to_string(parameters.numPointLights));
			str = replace_all(str, "NUM_HEMI_LIGHTS", std::to_string(parameters.numHemiLights));
			str = replace_all(str, "NUM_DIR_LIGHT_SHADOWS", std::to_string(parameters.numDirLightShadows));
			str = replace_all(str, "NUM_SPOT_LIGHT_SHADOWS", std::to_string(parameters.numSpotLightShadows));
			str = replace_all(str, "NUM_POINT_LIGHT_SHADOWS", std::to_string(parameters.numPointLightShadows));

			return str;
		}

		std::string GLProgram::replaceClippingPlaneNums(std::string& str, const ProgramParameters& parameters)
		{
			int numClipingPlanes = parameters.numClippingPlanes;
			int numClipIntersection = parameters.numClippingPlanes;
			str = replace_all(str, "NUM_CLIPPING_PLANES", std::to_string(numClipingPlanes));
			str = replace_all(str, "UNION_CLIPPING_PLANES", std::to_string(numClipingPlanes - numClipingPlanes));

			return str;

		}
		static const std::string includePattern = R"([ \t]*#include +<([\w\d./]+)>)";
			//"[ \\t] * #include + <([\\w\\d./]+)>";

		std::string GLProgram::resolveIncludes(std::string& source)
		{
			std::regex regx(includePattern);

			std::string localSource = source;
			auto words_begin = std::sregex_iterator(source.begin(), source.end(), regx);
			auto words_end = std::sregex_iterator();
			if (words_begin != words_end) {
				for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
					smatch match = *i;
					string result = includeReplacer(match);
					localSource = replace_all(localSource, match[0], result);				
				}
			}
			return localSource;
		}
		std::string GLProgram::includeReplacer(std::smatch& match)
		{


			if (match.size() > 1) {
				stringstream localstream;
				for (unsigned ctr = 1; ctr < match.size(); ctr++) {
					std::string include = match[ctr];
					std::string source = getShaderChunk(shaderlib_name::get(include));
					if (source.empty() || source.size() == 1) continue;					
					string result = resolveIncludes(source);
					source = replace_all(source, match[0], result);
					localstream << source;
				}

				return localstream.str();
			}
			else {
				return string();
			}
		}
		static const std::string loopPattern = R"(#pragma unroll_loop_start[\s]+?for \( int i \= (\d+)\; i < (\d+)\; i \+\+ \) \{([\s\S]+?)(?=\})\}[\s]+?#pragma unroll_loop_end)";
			//"(#pragma unroll_loop[\\s]+?for \\( int i \\= (\\d+)\\; i < (\\d+)\\; i \\+\\+ \\) \\{([\\s\\S]+?)(?=\\})\\}";

		std::string GLProgram::unrollLoops(const std::string& source)
		{
			std::regex regx(loopPattern);

			std::string localSource = source;
			auto words_begin = std::sregex_iterator(source.begin(), source.end(), regx);
			auto words_end = std::sregex_iterator();
			if (words_begin != words_end) {
				for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
					smatch match = *i;
					string result = loopReplacer(match);
					localSource = replace_all(localSource, match[0], result);
				}
			}
			return localSource;
		}

		std::string GLProgram::resolveIncludeUX(const std::string& source)
		{
			static const std::regex rex(includePattern);

			string result;

			sregex_iterator rex_it(source.begin(), source.end(), rex);
			sregex_iterator rex_end;
			size_t pos = 0;

			while (rex_it != rex_end) {
				std::smatch match = *rex_it;
				result.append(source, pos, match.position(0) - pos);
				pos = match.position(0) + match.length(0);

				std::ssub_match sub = match[1];
				string r = getShaderChunk(shaderlib_name::get(sub.str()));
				if (r.empty()) {
					stringstream ss;
					ss << "unable to resolve #include <" << sub.str() << ">";
					throw logic_error(ss.str());
				}
				result.append(r);
				rex_it++;
			}
			if (pos == 0) return source;
			else {
				result.append(source, pos, source.length());
				return result;
			}
		}

		std::string GLProgram::unrollLoopsUX(const std::string& source)
		{
			static const regex rex(R"(for \( int i = (\d+)\; i < (\d+); i \+\+ \) \{[\r\n]?([\s\S]+?)(?=\})\})");
			static const regex rex2(R"(\[ i \])");

			stringstream unroll;
			sregex_iterator rex_it(source.begin(), source.end(), rex);
			sregex_iterator rex_end;

			auto it_start = source.begin();

			while (rex_it != rex_end) {

				smatch match = *rex_it;

				for (auto it_end = source.begin() + match.position(); it_start != it_end; it_start++) unroll << *it_start;

				int start = atoi(match[1].str().c_str());
				int end = atoi(match[2].str().c_str());
				ssub_match snippet = match[3];

				for (int i = start; i < end; i++) {
					stringstream ss2;
					ss2 << "[ " << i << " ]";
					unroll << regex_replace(snippet.str(), rex2, ss2.str());
				}
				rex_it++;
				it_start = source.begin() + match.position() + match.length();
			}
			for (; it_start < source.end(); it_start++) unroll << *it_start;

			return unroll.str();
		}

		std::string GLProgram::loopReplacer(std::smatch& match)
		{
			stringstream ss;

			if (match.size() > 1) {
				int start = std::stoi(match[1]);
				int end = std::stoi(match[2]);
				string snippet = match[3];
				for (int i = start;i < end;i++) {
					string snippet1 = replace_all(snippet,"[ i ]", "[ " + std::to_string(i) + " ]");
					snippet1 = replace_all(snippet1, "UNROLLED_LOOP_INDEX", std::to_string(i));

					ss << snippet1 << endl;
				}
			}

			return ss.str();
		}

		std::string GLProgram::resolveIncludesRX(std::string& source) {
			pcrecpp::RE re(includePattern);
			pcrecpp::StringPiece input(source);
			std::string match;
			std::string localSource = source;
			while (re.FindAndConsume(&input, &match)) {

				string result = includeReplacerRX(match);
				pcrscpp_rs.add_job(includePattern, result, "");
				pcrscpp_rs.replace_inplace(localSource);
				
				pcrscpp_rs.pop_job_back();
			}
			return localSource;
		}
		std::string GLProgram::includeReplacerRX(std::string& match) {
			std::string localSource = getShaderChunk(shaderlib_name::get(match));
			std::string result;
			result = resolveIncludesRX(localSource);
			pcrscpp_rs.add_job(includePattern, result, "");
			pcrscpp_rs.replace_inplace(result);
			pcrscpp_rs.pop_job_back();
			
			return result;
		}

		std::string GLProgram::generatePrecision(const ProgramParameters& parameter)
		{
			stringstream ss;
			ss << "precision " << parameter.precision << " float; " << endl;
			ss << "precision " + parameter.precision + " int; "<<endl;

			if (parameter.precision == "highp")
			{
				ss << "#define HIGH_PRECISION" << endl;
			}
			else if (parameter.precision=="mediump")
			{
				ss <<"#define MEDIUM_PRECISION"<<endl;
			}
			else if (parameter.precision =="lowp")
			{
				ss<<"#define LOW_PRECISION"<<endl;
			}

			return ss.str();
		}

		std::string GLProgram::generateShadowMapTypeDefine(const ProgramParameters& parameters)
		{

			if(parameters.shadowMapType==ShadowMapType::PCFShadowMap)
				return "SHADOWMAP_TYPE_PCF";
			else if(parameters.shadowMapType ==ShadowMapType::PCFSoftShadowMap)
				return "SHADOWMAP_TYPE_PCF_SOFT";
			else if(parameters.shadowMapType == ShadowMapType::VSMShadowMap)
				return "SHADOWMAP_TYPE_VSM";
			else
				return "SHADOWMAP_TYPE_BASIC";
		}

		std::string GLProgram::generateEnvMapTypeDefine(const ProgramParameters& parameters)
		{
			std::string envMapTypeDefine = "ENVMAP_TYPE_CUBE";
			if (parameters.envMap)
			{
				TextureMapping envMapMode = parameters.envMapMode;
				switch (envMapMode)
				{

				case TextureMapping::CubeReflectionMapping: //Constants.CubeReflectionMapping:
				case TextureMapping::CubeRefractionMapping:
					envMapTypeDefine = "ENVMAP_TYPE_CUBE";
					break;

				case TextureMapping::CubeUVReflectionMapping:
				case TextureMapping::CubeUVRefractionMapping:
					envMapTypeDefine = "ENVMAP_TYPE_CUBE_UV";
					break;

				case TextureMapping::EquirectangularReflectionMapping:
				case TextureMapping::EquirectangularRefractionMapping:
					envMapTypeDefine = "ENVMAP_TYPE_EQUIREC";
					break;

				case TextureMapping::SphericalReflectionMapping:
					envMapTypeDefine = "ENVMAP_TYPE_SPHERE";
					break;

				}

			}

			return envMapTypeDefine;
		}

		std::string GLProgram::generateEnvMapModeDefine(const ProgramParameters& parameters)
		{
			
			if (parameters.envMap) {
				switch (parameters.envMapMode) {

				case TextureMapping::CubeRefractionMapping:
				case TextureMapping::EquirectangularRefractionMapping:
					return "ENVMAP_MODE_REFRACTION";
				default :
					return "ENVMAP_MODE_REFLECTION";

				}

			}

			return "ENVMAP_MODE_REFLECTION";
		}

		std::string GLProgram::generateEnvMapBlendingDefine(const ProgramParameters& parameters)
		{
			//var envMapBlendingDefine = "ENVMAP_BLENDING_NONE";
			if (parameters.envMap) {
				//int combine = (int)parameters["combine"];
				switch (parameters.combine) {

					case Combine::MultiplyOperation:
					return "ENVMAP_BLENDING_MULTIPLY";
					break;

					case Combine::MixOperation:
					return "ENVMAP_BLENDING_MIX";
					break;

					case Combine::AddOperation:
					return  "ENVMAP_BLENDING_ADD";
					break;
				default :
					return "ENVMAP_BLENDING_NONE";

				}

			}		
			return "ENVMAP_BLENDING_NONE";
		}

		

		GLUniforms::ptr GLProgram::getUniforms()
		{
			// TODO: 여기에 return 문을 삽입합니다.
			if (cachedUniforms == nullptr) {
				cachedUniforms = GLUniforms::create(renderer.textures,program);
			}
			return cachedUniforms;
		}

		std::unordered_map<std::string, GLint>& GLProgram::getAttributes()
		{
			// TODO: 여기에 return 문을 삽입합니다.
			if (cachedAttributes.size() == 0) {
				fetchAttributeLocation(program, cachedAttributes);
			}
			return cachedAttributes;
		}

		std::string GLProgram::addLineNumbers(const std::string& code)
		{
			std::vector<std::string> lines = split(code, '\n');
			stringstream ss;
			for (unsigned i = 0;i < lines.size();i++) {
				
				ss << (i + 1) << ":" << lines[i]<<endl;
				lines[i] = ss.str();
			}
			return ss.str();
		}
	}
}
