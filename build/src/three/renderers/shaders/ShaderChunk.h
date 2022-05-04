#ifndef SHADER_CHUNK_THREE_H
#define SHADER_CHUNK_THREE_H

#include <string>
#include <unordered_map>
#include <three/Constants.h>
namespace three {
	namespace gl {

		namespace shaderlib_name {
			ShaderLibID get(std::string name);
		}
		const char* getShaderChunk(ShaderLibID chunk);
	}
}
#endif