#ifndef POINTLIGHT_HELPER_H
#define POINTLIGHT_HELPER_H
#include <three/objects/Mesh.h>
#include <three/lights/Light.h>
#include <three/geometries/SphereGeometry.h>
#include <three/materials/MeshBasicMaterial.h>
namespace three {
	class PointLightHelper : public Mesh {
	public:
		using ptr = std::shared_ptr<PointLightHelper>;
		Light::ptr light;
		Color color;
		PointLightHelper(const Light::ptr& light, float sphereSize=1.0f, Color color=Color()):Mesh(),light(light),color(color) {
			this->type = "PointLightHelper";
			this->light->updateMatrixWorld();
			this->matrix = light->matrixWorld;
			this->matrixAutoUpdate = false;

			auto geometry = SphereBufferGeometry::create(sphereSize, 4, 2);
			auto material = MeshBasicMaterial::create();

			this->set(geometry, material);

			material->wireframe = true;
			material->fog = false;
			material->toneMapped = false;

			this->update();
		}
		static ptr create(const Light::ptr& light, float sphereSize=1.0f, Color color=Color()) {
			return std::make_shared<PointLightHelper>(light, sphereSize, color);
		}
		void update() {
			if (!color.isNull()) {
				this->material->color = color;
			}
			else {
				this->material->color = light->color;
			}
		}
	};
}
#endif 