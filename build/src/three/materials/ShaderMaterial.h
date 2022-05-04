#ifndef SHADER_MATERIAL_THREE_H
#define SHADER_MATERIAL_THREE_H
#include <three/materials/Material.h>

namespace three {
	class ShaderMaterial : public Material{
		
	public:

		using ptr = std::shared_ptr<ShaderMaterial>;


		std::unordered_map<std::string, std::vector<float>> defaultAttributeValues;

		ShaderMaterial();

		ShaderMaterial(const ShaderMaterial& source) : Material(source){
			type = "ShaderMaterial";
			name = "ShadowMaterial";
			fragmentShader = source.fragmentShader;
			vertexShader = source.vertexShader;
			isShaderMaterial = source.isShaderMaterial;
			//uniforms->copy(*source.uniforms);
			wireframe = source.wireframe;
			wireframeLinewidth = source.wireframeLinewidth;
			lights = source.lights;
			clipping = source.clipping;
			skinning = source.skinning;
			morphTargets = source.morphTargets;
			morphNormals = source.morphNormals;
			extensions = source.extensions;
			indexOfAttributeName = source.indexOfAttributeName;
			extensions = source.extensions;
		}
		ShaderMaterial& copy(const ShaderMaterial& source) {
			Material::copy(source);
			type = source.type;
			fragmentShader = source.fragmentShader;
			vertexShader = source.vertexShader;

			//uniforms->copy(*source.uniforms);
			wireframe = source.wireframe;
			wireframeLinewidth = source.wireframeLinewidth;
			lights = source.lights;
			clipping = source.clipping;
			skinning = source.skinning;
			morphTargets = source.morphTargets;
			morphNormals = source.morphNormals;
			extensions = source.extensions;
			indexOfAttributeName = source.indexOfAttributeName;
			extensions = source.extensions;
			return *this;
		}
		static ptr create() {
			return std::make_shared<ShaderMaterial>();
		}
	};
}
#endif