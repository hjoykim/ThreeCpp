#include "ShaderMaterial.h"
#include <three/renderers/shaders/ShaderChunk.h>
#include <three/renderers/shaders/ShaderLib.h>
namespace three {
	ShaderMaterial::ShaderMaterial() : Material()
	{
		type = "ShaderMaterial";
		name = "ShaderMaterial";
		isShaderMaterial = true;

		uniforms = std::make_shared<gl::UniformValues>();

		vertexShader.assign(gl::getShaderChunk(gl::shaderlib_name::get("default_vertex")));
		fragmentShader.assign(gl::getShaderChunk(gl::shaderlib_name::get("default_fragment")));

		linewidth = 1;

		wireframe = false;
		wireframeLinewidth = 1;

		fog = false;
		lights = false;
		clipping = false;

		skinning = false;
		morphTargets = false;
		morphNormals = false;

		extensions.derivatives = false;
		extensions.fragDepth = false;
		extensions.drawBuffers = false;
		extensions.shaderTextureLOD = false;

		defaultAttributeValues["color"] = { 1,1,1 };
		defaultAttributeValues["uv"] = { 0,0 };
		defaultAttributeValues["uv2"] = { 0,0 };
	}
}
