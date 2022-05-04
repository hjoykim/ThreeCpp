#ifndef SPRITE_MATERIAL_THREE_H
#define SPRITE_MATERIAL_THREE_H
#include <string>
#include <three/materials/Material.h>
#include <three/Constants.h>
namespace three {
	class SpriteMaterial : public Material {
	public:
		SpriteMaterial() :Material() {
			type = "SpriteMaterial";
			name = "SpriteMaterial";
			shaderId = "sprite";

			color.set(0xffffff);// = new Color(0xffffff);

			//map = null;

			//alphaMap = null;

			rotation = 0;

			sizeAttenuation = true;

			transparent = true;
		}
		SpriteMaterial(const SpriteMaterial& source) :Material(source){
			color.copy(source.color);

			map = source.map;

			alphaMap = source.alphaMap;

			rotation = source.rotation;

			sizeAttenuation = source.sizeAttenuation;
		}
		virtual ~SpriteMaterial() = default;

		virtual SpriteMaterial* clone() {
			return new SpriteMaterial(*this);
		}

		virtual SpriteMaterial& copy(const SpriteMaterial& source) {
			Material::copy(source);

			color.copy(source.color);

			map = source.map;

			alphaMap = source.alphaMap;

			rotation = source.rotation;

			sizeAttenuation = source.sizeAttenuation;

			return *this;
		}
	};
}
#endif