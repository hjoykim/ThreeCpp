#ifndef LINE_BASIC_MATERIAL_THREE_H
#define LINE_BASIC_MATERIAL_THREE_H
#include <three/materials/Material.h>
#include <three/math/Color.h>
#include <string>
#include <iostream>
namespace three {
	class LineBasicMaterial : public Material {
	public:	
		using ptr = std::shared_ptr<LineBasicMaterial>;

		LineBasicMaterial()  : Material() {
			type = "LineBasicMaterial";
			name = "LineBasicMaterial";
			shaderId = "basic";
			color.set(0xffffff);
			linewidth = 1;
			linecap = "round";
			linejoin = "round";
		}
		LineBasicMaterial(const LineBasicMaterial& source) : Material(source) {
			
			color.copy(source.color);
			
			linewidth = source.linewidth;
			linecap = source.linecap;
			linejoin = source.linejoin;
		}
		LineBasicMaterial(const Color& color) : LineBasicMaterial() {
			this->color.copy(color);
		}
		
		static ptr create() {
			return std::make_shared<LineBasicMaterial>();
		}

		static ptr create(const Color& color) {
			return std::make_shared<LineBasicMaterial>(color);
		}

		virtual LineBasicMaterial* clone() {
			return new LineBasicMaterial(*this);
		}
		virtual LineBasicMaterial& copy(const LineBasicMaterial& source) {
			Material::copy(source);
			
			color.copy(source.color);
			
			linewidth = source.linewidth;
			linecap = source.linecap;
			linejoin = source.linejoin;
			return *this;
		}
		virtual LineBasicMaterial& copy(const Material& source) override {
			Material::copy(source);
			type = "LineBasicMaterial";
			name = "LineBasicMaterial";
			shaderId = "basic";
			return *this;
		}
	};
}
#endif