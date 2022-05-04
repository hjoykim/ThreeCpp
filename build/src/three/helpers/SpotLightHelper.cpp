#include "SpotLightHelper.h"
namespace three {
	SpotLightHelper::SpotLightHelper(SpotLight::ptr& light, Color& color) :Object3D(), light(light), color(color)
	{

		type = "SpotLightHelper";

		light->updateMatrixWorld();

		matrix = light->matrixWorld;

		matrixAutoUpdate = false;


		BufferGeometry::ptr geometry = BufferGeometry::create();

		std::vector<float> positions =
		{
			0, 0, 0, 0, 0, 1,
				0, 0, 0, 1, 0, 1,
				0, 0, 0, -1, 0, 1,
				0, 0, 0, 0, 1, 1,
				0, 0, 0, 0, -1, 1
		};

		for (int i = 0, j = 1, l = 32; i < l; i++, j++)
		{
			float p1 = (i / l) * M_PI * 2;
			float p2 = (j / l) * M_PI * 2;

			positions.push_back((float)cos(p1));
			positions.push_back((float)sin(p1));
			positions.push_back(1);
			positions.push_back((float)cos(p2));
			positions.push_back((float)sin(p2));
			positions.push_back(1);

		}
		geometry->setAttribute(AttributeName::position, BufferAttribute<float>::create(positions, 3));

		auto material = LineBasicMaterial::create();
		material->fog = false;
		material->toneMapped = false;

		cone = LineSegments::create(geometry, material);

		add(cone);

		update();
	}
	void SpotLightHelper::update()
	{
		
		light->updateMatrixWorld();
		this->matrix.copy(light->matrix);
		this->matrixWorld.copy(light->matrixWorld);
		float coneLength = light->distance != 0 ? light->distance : 1000;
		float coneWidth = coneLength * tan(light->angle);

		cone->scale.set(coneWidth, coneWidth, coneLength);

		_vector.setFromMatrixPosition(light->target->matrixWorld);

		cone->lookAt(_vector);

		if (!color.isNull()) {

			cone->material->color = color;

		}
		else {

			cone->material->color = light->color;

		}
	}
}
