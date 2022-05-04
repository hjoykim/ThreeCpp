#ifndef GLCAPABILITIES_THREE_H
#define GLCAPABILITIES_THREE_H
#include <string>
#include <three/Constants.h>
#include <three/renderers/gl/GLExtensions.h>
namespace three {
	namespace gl {
		class GLCapabilities {
		public:
			using ptr = std::shared_ptr<GLCapabilities>;

			struct GLCapabilitiesParameters {
				bool logarithmicDepthBuffer = false;
				std::string precision;
			};

			std::string precision;

			bool logarithmicDepthBuffer = false;

			GLint maxTextures = 0;

			GLint maxVertexTextures = 0;

			GLsizei maxTextureSize = 0;

			GLsizei maxCubemapSize = 0;

			GLint maxAttributes = 0;

			GLint maxVertexUniforms = 0;

			GLint maxVaryings = 0;

			GLint maxFragmentUniforms = 0;

			bool vertexTextures = false;

			bool floatFragmentTextures =false;

			bool floatVertexTextures = false;

			bool isGL2 = false;

			GLint maxAnisotropy = -1;

			GLint maxSamples = 0;

			GLExtensions::ptr extensions;

			GLCapabilitiesParameters parameters;

			GLCapabilities(GLExtensions::ptr& extensions, const GLCapabilitiesParameters& parameters) : extensions(extensions), parameters(parameters)
			{
				
				std::string maxPrecision = getMaxPrecision(precision);

				if (maxPrecision != this->precision) {
					this->precision = maxPrecision;
				}
						
				isGL2 = true;// extensions->get(Extension::ARB_ES3_compatibility);

				this->logarithmicDepthBuffer = parameters.logarithmicDepthBuffer == true;

				glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);
				glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextures);
				glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
				glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &maxCubemapSize);
				glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
				glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUniforms);
				glGetIntegerv(GL_MAX_VARYING_VECTORS, &maxVaryings);
				glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragmentUniforms);

				vertexTextures = maxVertexTextures > 0;
				floatFragmentTextures = isGL2 || !extensions->get(Extension::OES_texture_float);
				floatVertexTextures = vertexTextures && floatFragmentTextures;

				glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

				maxSamples = isGL2 ? maxSamples : 0;

				checkError();
			}

			virtual ~GLCapabilities() = default;

			float getMaxAnisotropy() {
				if (maxAnisotropy < 0) return maxAnisotropy;

				/*if (extensions.get(Extension::EXT_texture_filter_anisotropic))
					glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
				else*/
					maxAnisotropy = 0;

				return maxAnisotropy;
			}
			std::string getMaxPrecision(std::string& precision) {
				GLint r[2], p;
				if (precision == "highp") {
					glGetShaderPrecisionFormat(GL_VERTEX_SHADER, GL_HIGH_FLOAT, r, &p);
					if (p > 0) {
						glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER, GL_HIGH_FLOAT, r, &p);
						if (p > 0)
							return "highp";
					}
					precision = "miduump";
				}
				if (precision == "mediump") {
					glGetShaderPrecisionFormat(GL_VERTEX_SHADER, GL_MEDIUM_FLOAT, r, &p);
					if (p > 0)
						return "mediump";
				}
				return "highp";
				//return "lowp";
			}
		};
	}
}
#endif
