#ifndef RAWSHADER_MATERIAL_THREE_H
#define RAWSHADER_MATERIAL_THREE_H
#include <three/materials/ShaderMaterial.h>
namespace three {
	class RawShaderMaterial : public ShaderMaterial {
	public:
		using ptr = std::shared_ptr<RawShaderMaterial>;
		RawShaderMaterial() :ShaderMaterial() {
			type = "RawShaderMaterial";
			isRawShaderMaterial = true;
		}
		static ptr create() {
			return std::make_shared<RawShaderMaterial>();
		}
	};
}
#endif
