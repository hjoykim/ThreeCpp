#ifndef SPOTLIGHT_HELPER_H
#define SPOTLIGHT_HELPER_H
#include <three/core/Object3D.h>
#include <three/lights/Light.h>
#include <three/objects/Line.h>
#include <three/materials/LineBasicMaterial.h>
namespace three {

	class SpotLightHelper : public Object3D {
	private:
		Vector3 _vector;
		
	public :
		SpotLight::ptr light;
		Color color;
		LineSegments::ptr cone;
		using ptr = std::shared_ptr<SpotLightHelper>;
		SpotLightHelper(SpotLight::ptr& light, Color& color);
		
		static ptr create(SpotLight::ptr& light, Color& color) {
			return std::make_shared<SpotLightHelper>(light, color);
		}
		void update();
		
	};
}
#endif