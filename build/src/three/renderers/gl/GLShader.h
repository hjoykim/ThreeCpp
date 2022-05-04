#ifndef GLSHADER_THREE_H
#define GLSHADER_THREE_H
#include <three/Constants.h>
#include <string>
#include <three/renderers/shaders/shaderLib.h>
namespace three {
	namespace gl {	
		//class uniformslib::LibUniformValues;
		
		class GLShader {
		public:
			
			using ptr = std::shared_ptr<GLShader>;

			unsigned type;

			std::string name="";

			std::string code ="";

			const char* vertexShader;

			const char* fragmentShader;

			unsigned shader = -1;
			
			UniformValues uniforms;

			GLShader() :type(GL_VERTEX_SHADER){}

			GLShader(const char* vertexShader, const char* fragmentShader, const LibUniformValues& uniforms)
				: type(GL_VERTEX_SHADER), vertexShader(vertexShader), fragmentShader(fragmentShader), uniforms(uniforms) {}

			GLShader(unsigned type, const std::string& code) {
				this->type = type;
				this->code = code;

				shader = glCreateShader(type);

				const char* shaderCode = code.c_str();
				glShaderSource(shader, 1, &shaderCode, NULL);
				glCompileShader(shader);
			}

			static ptr create() {
				return std::make_shared<GLShader>();
			}
			UniformValues& getUniforms() {
				return uniforms;
			}
					
		};
	}
}
#endif