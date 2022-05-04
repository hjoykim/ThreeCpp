#include "GLUniforms.h"
#include <typeinfo>
#include <regex>
#include <three/renderers/gl/GLTextures.h>
#include <three/renderers/GLCubeRenderTarget.h>
#include <three/textures/CubeTexture.h>
#include <three/scenes/Fog.h>
#include <unordered_map>
#include <iostream>
#include <three/renderers/shaders/ShaderLib.h>
namespace three {
	namespace gl {
		/*namespace uniformname {
		#define MATCH_NAME(nm) {#nm, UniformName::nm}*/

		//	UniformName get(const std::string& name)
		//	{
		//		static const std::unordered_map<std::string, UniformName> string_to_name{
		//			MATCH_NAME(unknown_name),
		//			MATCH_NAME(clippingPlanes),
		//			MATCH_NAME(ambientLightColor),
		//			MATCH_NAME(lightProbe),
		//			MATCH_NAME(directionalLights),
		//			MATCH_NAME(directionalLightShadows),
		//			MATCH_NAME(spotLights),
		//			MATCH_NAME(spotLightShadows),
		//			MATCH_NAME(rectAreaLights),
		//			MATCH_NAME(pointLights),
		//			MATCH_NAME(pointLightShadows),
		//			MATCH_NAME(hemisphereLights),
		//			MATCH_NAME(directionalShadowMap),
		//			MATCH_NAME(directionalShadowMatrix),
		//			MATCH_NAME(spotShadowMap),
		//			MATCH_NAME(spotShadowMatrix),
		//			MATCH_NAME(pointShadowMap),
		//			MATCH_NAME(pointShadowMatrix),
		//			MATCH_NAME(projectionMatrix),
		//			MATCH_NAME(logDepthBufFC),
		//			MATCH_NAME(cameraPosition),
		//			MATCH_NAME(isOrthographic),
		//			MATCH_NAME(viewMatrix),
		//			MATCH_NAME(bindMatrix),
		//			MATCH_NAME(bindMatrixInverse),
		//			MATCH_NAME(boneTexture),
		//			MATCH_NAME(boneTextureSize),
		//			MATCH_NAME(boneMatrices),
		//			MATCH_NAME(receiveShadow),
		//			MATCH_NAME(toneMappingExposure),
		//			MATCH_NAME(toneMappingWhitePoint),
		//			MATCH_NAME(center),
		//			MATCH_NAME(modelViewMatrix),
		//			MATCH_NAME(normalMatrix),
		//			MATCH_NAME(modelMatrix),
		//			MATCH_NAME(fogColor),
		//			MATCH_NAME(fogNear),
		//			MATCH_NAME(fogFar),
		//			MATCH_NAME(fogExp2),
		//			MATCH_NAME(fogDensity),
		//			MATCH_NAME(opacity),
		//			MATCH_NAME(diffuse),
		//			MATCH_NAME(emissive),
		//			MATCH_NAME(map),
		//			MATCH_NAME(alphaMap),
		//			MATCH_NAME(specularMap),
		//			MATCH_NAME(envMap),
		//			MATCH_NAME(flipEnvMap),
		//			MATCH_NAME(reflectivity),
		//			MATCH_NAME(refractionRatio),
		//			MATCH_NAME(maxMipLevel),
		//			MATCH_NAME(lightMap),
		//			MATCH_NAME(lightMapIntensity),
		//			MATCH_NAME(aoMap),
		//			MATCH_NAME(aoMapIntensity),
		//			MATCH_NAME(uvTransform),
		//			MATCH_NAME(uv2Transform),
		//			MATCH_NAME(emissiveMap),
		//			MATCH_NAME(specular),
		//			MATCH_NAME(shininess),
		//			MATCH_NAME(gradientMap),
		//			MATCH_NAME(bumpMap),
		//			MATCH_NAME(bumpScale),
		//			MATCH_NAME(normalMap),
		//			MATCH_NAME(normalScale),
		//			MATCH_NAME(displacementMap),
		//			MATCH_NAME(displacementScale),
		//			MATCH_NAME(displacementBias),
		//			MATCH_NAME(clearcoat),
		//			MATCH_NAME(clearcoatRoughness),
		//			MATCH_NAME(sheen),
		//			MATCH_NAME(clearcoatMap),
		//			MATCH_NAME(clearcoatRoughnessMap),
		//			MATCH_NAME(clearcoatNormalScale),
		//			MATCH_NAME(clearcoatNormalMap),
		//			MATCH_NAME(transparency),
		//			MATCH_NAME(roughness),
		//			MATCH_NAME(metalness),
		//			MATCH_NAME(roughnessMap),
		//			MATCH_NAME(metalnessMap),
		//			MATCH_NAME(envMapIntensity),
		//			MATCH_NAME(matcap),
		//			MATCH_NAME(referencePosition),
		//			MATCH_NAME(nearDistance),
		//			MATCH_NAME(farDistance),
		//			MATCH_NAME(dashSize),
		//			MATCH_NAME(totalSize),
		//			MATCH_NAME(scale),
		//			MATCH_NAME(size),
		//			MATCH_NAME(rotation),
		//			MATCH_NAME(direction),
		//			MATCH_NAME(skyColor),
		//			MATCH_NAME(groundColor),
		//			MATCH_NAME(position),
		//			MATCH_NAME(color),
		//			MATCH_NAME(distance),
		//			MATCH_NAME(coneCos),
		//			MATCH_NAME(penumbraCos),
		//			MATCH_NAME(decay),
		//			MATCH_NAME(shadowBias),
		//			MATCH_NAME(shadowNormalBias),
		//			MATCH_NAME(shadowRadius),
		//			MATCH_NAME(shadowMapSize),
		//			MATCH_NAME(width),
		//			MATCH_NAME(height),
		//			MATCH_NAME(shadowCameraNear),
		//			MATCH_NAME(shadowCameraFar),
		//			MATCH_NAME(tEquirect),
		//			MATCH_NAME(t2D),
		//			MATCH_NAME(shadow_pass),
		//			MATCH_NAME(resolution),
		//			MATCH_NAME(radius),
		//			MATCH_NAME(morphTargetBaseInfluence),
		//			MATCH_NAME(morphTargetInfluences),
		//			MATCH_NAME(occlusionMap),
		//			MATCH_NAME(screenPosition)

		//		};

		//		auto found = string_to_name.find(name);
		//		if (found != string_to_name.end()) return found->second;

		//		//std::cout << "UniformName " << name  << " was not defined. it set to unknown_name" << std::endl;
		//		return UniformName::unknown_name;
		//	}
		//}

		namespace uniform {
			Texture emptyTexture;
			Texture emptyTexture2dArray;
			Texture emptyTexture3d;
			Texture emptyCubeTexture;

			std::unordered_map<size_t, std::vector<float>> arrayCacheF32;
			std::unordered_map<size_t, std::vector<int32_t>> arrayCacheI32;

			std::array<float, 16> mat4Array;
			std::array<float, 9> mat3Array;
			std::array<float, 4> mat2Array;
		}
		using namespace uniform;
		std::vector<GLuint> GLUniform::allocTextUnits(GLuint n)
		{
			std::vector<GLuint> units(n);
			units.clear();

			for (size_t i = 0; i < n; ++i)
				units.push_back(textures->allocateTextureUnit());

			return units;
		}

		void GLUniform::setValue(bool b)
		{
			glUniform1i(addr, (int)b);
		}

		void GLUniform::setValue(const std::vector<unsigned char>& b)
		{
			glUniform1iv(addr, b.size(), (GLint*)b.data());
		}

		void GLUniform::setValue(GLfloat f)
		{
			glUniform1f(addr, f);
		}

		void GLUniform::setValue(GLint i)
		{
			switch (type) {
			case UniformType::Float:
				glUniform1f(addr, (float)i);
				break;
			case UniformType::Int:
				glUniform1i(addr, i);
			}
		}

		void GLUniform::setValue(GLuint u)
		{
			switch (type) {
			case UniformType::Float:
				glUniform1f(addr, (float)u);
				break;
			case UniformType::Int:
				glUniform1i(addr, u);
			}
		}

		void GLUniform::setValue(const math::Color& c)
		{
			glUniform3f(addr, c.r, c.g, c.b);
		}

		void GLUniform::setValue(const math::Vector2& v)
		{
			glUniform2fv(addr, 1, v.elements);
		}

		void GLUniform::setValue(const math::Vector3& v)
		{
			glUniform3fv(addr, 1, v.elements);
		}

		void GLUniform::setValue(const math::Vector4& v)
		{
			glUniform4fv(addr, 1, v.elements);
		}

		void GLUniform::setValue(const math::Matrix3& v)
		{
			glUniformMatrix3fv(addr, 1, GL_FALSE, v.elements);
		}

		void GLUniform::setValue(const math::Matrix4& v)
		{
			glUniformMatrix4fv(addr, 1, GL_FALSE, v.elements);
		}

		void GLUniform::setValue(const std::vector<float>& v)
		{
			glUniform1fv(addr, v.size(), v.data());
		}		

		void GLUniform::setValue(const std::vector<GLuint>& v) {
			glUniform2iv(addr, v.size(), (GLint*)&v[0]);
		}

		void GLUniform::setValue(const Texture::ptr& t)
		{
			GLuint unit = textures->allocateTextureUnit();

			glUniform1i(addr, unit);
			
			switch (type) {
			case UniformType::Sampler2D:
				textures->safeSetTexture2D(*t, unit);
				break;
			case UniformType::Sampler3D:
				textures->setTexture3D(*t, unit);
				break;
			case UniformType::SamplerCube:
				textures->safeSetTextureCube(*t, unit);
				break;
			case UniformType::Sampler2DArray:
				textures->setTexture2DArray(*t, unit);
				break;
			}
		}

		

		void GLUniform::setValue(const std::vector<math::Matrix4>& v)
		{
			glUniformMatrix4fv(addr, v.size(), GL_FALSE, reinterpret_cast<const GLfloat*>(v.data()));
		}

		void GLUniform::setValue(const std::vector<Texture::ptr>& t)
		{
			int n = t.size();

			/*if (n == 1) {
				setValue(t[0], textures);
				return;
			}*/

			auto units = allocTextUnits(n);

			glUniform1iv(addr, (GLsizei)n, reinterpret_cast<const GLint*>(units.data()));

			for (int i = 0; i < n; i++)
			{
				if (type == UniformType::Sampler2D)
					//setValueT1Array
					textures->safeSetTexture2D(*t[i], units[i]);
				else if (type == UniformType::SamplerCube)
					//setValueT6Array
					textures->safeSetTextureCube(*t[i], units[i]);
			}

		}
		
		void GLUniform::setValue(const std::vector<math::Vector2>& v) {
			//std::vector<float> array2f = flatten(v);
			//setValue(array2f);

			glUniform2fv(addr, v.size(), reinterpret_cast<const float*>(v.data()));
		}

		void GLUniform::setValue(const std::vector<math::Vector3>& v) {
			//std::vector<float> array3f = flatten(v);
			//setValue(array3f);
		
			glUniform3fv(addr, v.size(), reinterpret_cast<const float*>(v.data()));
			int error = glGetError();
		}

		void GLUniform::setValue(const std::vector<math::Vector4>& v) {
			//std::vector<float> array4f = flatten(v);
			//setValue(array4f);
			glUniform4fv(addr, v.size(), reinterpret_cast<const float*>(v.data()));
		}

		void GLUniform::setValue(const std::vector<Matrix3>& v)
		{
			glUniformMatrix3fv(addr, v.size(), GL_FALSE, reinterpret_cast<const float*>(v.data()));
		}

		void GLUniforms::initGLUniforms(GLuint program) 
		{
			this->program = program;

			int n = 0;

			glGetProgramiv(this->program, GL_ACTIVE_UNIFORMS, &n);

			for (unsigned i = 0; i < n; i++) {
				GLint size;
				GLsizei bufSize = 1024;
				GLsizei length;
				GLchar uniformName[256];
				GLenum type;
				glGetActiveUniform(this->program, i, bufSize, &length, &size, &type, uniformName);
				int addr = glGetUniformLocation(this->program, uniformName);

				parseUniform(std::string(uniformName), (UniformType)type, addr, this);	
				//std::cout << uniformName << ":" << addr << std::endl;
			}

		}
		

		static const std::string uniformPattern = R"(([\w\d_]+)(\])?(\[|\.)?)";
		void GLUniforms::parseUniform(const std::string& name, UniformType activeInfo, const GLint addr, GLUniformContainer* container)
		{
			static std::regex regx(uniformPattern);
			std::sregex_iterator rex_it(name.cbegin(), name.cend(), regx);
			std::sregex_iterator rex_end;

			while (rex_it != rex_end) {
				std::smatch match = *rex_it;
				//std::string id = match[1];
				bool idIsIndex = match[2] == "]";
				std::string subscript = match[3];

				std::string id = match[1];
				int idInt = -1;

				if (idIsIndex) {
					std::stringstream ssInt(id);
					ssInt >> idInt;
					idInt = (ssInt.fail()) ? 0 : idInt; // convert to string
				}
				
				int matchesSize = match[1].length();
				int matchPosition = match.position(1);
				if (subscript.empty() || subscript == "[" && matchesSize + match.position(1) == (name.size()-3)) {
					// bare name or "pure" bottom-level array "[0]" suffix
					if (!match[3].matched) {
						container->add(GLUniform::create(this->textures,id, activeInfo, addr));
						
					}
					else {
						container->add(PureArrayUniform::create(this->textures,id, activeInfo, addr));
					}
				}
				else {
					// step into inner node / create it in case it doesn't exist
					if (container->seq.count(id)==0) {
						StructuredUniform::ptr next = StructuredUniform::create(this->textures,id, activeInfo, addr);
						//auto tempPtr = StructureUniform::create(id, activeInfo, addr);		
						container->add(next);
					}
					
					container = container->seq[id]->asGLContainer();
					assert(container != nullptr);
				}
				rex_it++;
			}
		}
		void GLUniforms::setProjectionMatrix(const math::Matrix4& projMatrix)
		{
			if (this->seq.count("projectionMatrix")) {				
				GLUniform::ptr u = seq["projectionMatrix"];
				glUniformMatrix4fv(u->addr, 1, GL_FALSE, projMatrix.elements);
			}
		}

		void GLUniforms::upload(const std::vector<GLUniform::ptr>& seq, UniformValues& values)
		{
			for (auto& uniform : seq) {
				if (values.contains(uniform->id)) {
					UniformValue& v = values[uniform->id];
					/*if (uniform->id == UniformName::directionalShadowMap)
						std::cout << "DirectionalShadowMap" << std::endl;*/
					if (uniform->uniformClassType == UniformClassType::SingleUniform)
						v.applyValue(uniform);
					else if (uniform->uniformClassType == UniformClassType::PureArrayUniform)
						v.applyValue(uniform);
					else {
						StructuredUniform::ptr structUniform = std::dynamic_pointer_cast<StructuredUniform>(uniform);
						structUniform->setValue(v);
					}

					/*UniformValue& v = values[uniform->id];
					v.applyValue(uniform);*/

				}
			}
		}
		std::vector<GLUniform::ptr> GLUniforms::seqWithValue(UniformValues& values)
		{
			std::vector<GLUniform::ptr> r;

			for (auto uniform = seq.begin(); uniform != seq.end();){
				if (values.contains(uniform->first))
					r.push_back(uniform->second);
					//r.push_back(uniform->second->clone(uniform->first));
				uniform++;
			}
			return r;
		}
		void StructuredUniform::setValue(UniformValue& v,int arrayIndex)
		{
			for (auto& uniformKey : seq) {
				auto& uniform = uniformKey.second;
				
				if (uniform->uniformClassType == UniformClassType::SingleUniform)
					v.applyValue(uniform,arrayIndex);
				else if (uniform->uniformClassType == UniformClassType::PureArrayUniform)
					v.applyValue(uniform,arrayIndex);
				else {
					int arrayIndex = -1;
					std::stringstream ssInt(uniform->id);
					ssInt >> arrayIndex;
					StructuredUniform::ptr structUniform = std::dynamic_pointer_cast<StructuredUniform>(uniform);
					if (arrayIndex < 0) arrayIndex = 0;
					structUniform->setValue(v,arrayIndex);
				}
			}
		}
}
}


