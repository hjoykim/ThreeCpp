#ifndef SHADOW_MATERIAL_THREE_H
#define SHADOW_MATERIAL_THREE_H
#include <string>
#include <three/textures/Texture.h>
#include <three/materials/Material.h>
#include <three/Constants.h>

namespace three {
	class ShadowMaterial : Material {
	public:
		ShadowMaterial() :Material() {
			type = "ShadowMaterial";
			name = "ShadowMaterial";
			shaderId = "shadow";
			color.set(0x000000);// = new Color(0x000000);
			transparent = true;
		}

		ShadowMaterial(const ShadowMaterial& source) : Material(source){
			color.copy(source.color);
		}

		virtual ~ShadowMaterial() = default;

		virtual ShadowMaterial* clone() {
			return new ShadowMaterial(*this);
		}

		virtual ShadowMaterial& copy(const ShadowMaterial& source) {
			Material::copy(source);
			color.copy(source.color);
			return *this;
		}
	};
}
#endif