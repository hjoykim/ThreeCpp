#include "LOD.h"
#include <three/cameras/Camera.h>
namespace three {

	LOD& LOD::addLevel(const Object3D::ptr& object, float distance)
	{
		// TODO: 여기에 return 문을 삽입합니다.

		if (distance < 0) distance = 0;

		distance = abs(distance);
		unsigned l = 0;
		for (l = 0;l < levels.size();l++) {
			if (distance < levels[l].distance) {
				break;
			}
		}
		LevelStruct level = LevelStruct(object, distance);

		if (l >= levels.size())
			levels.push_back(level);
		else
			levels.insert(levels.begin() + l, level);

		add(object);

		return *this;
	}
	std::shared_ptr<Object3D> LOD::getObjectForDistance(float distance)
	{

		if (levels.size() > 0) {

			unsigned i=1;
			for (unsigned i = 1;i< levels.size(); i++) {

				if (distance < levels[i].distance) {

					break;

				}

			}

			return levels[i - 1].object;

		}

		return nullptr;
	}
	void LOD::update(const Camera& camera)
	{

		if (levels.size() > 1) {

			_v1.setFromMatrixPosition(camera.matrixWorld);
			_v2.setFromMatrixPosition(this->matrixWorld);

			float distance = _v1.distanceTo(_v2) / camera.zoom;

			levels[0].object->visible = true;
			unsigned i;
			unsigned l = levels.size();
			for (i = 1;i< levels.size(); i++) {

				if (distance >= levels[i].distance) {

					levels[i - 1].object->visible = false;
					levels[i].object->visible = true;

				}
				else {

					break;

				}

			}

			_currentLevel = i - 1;

			for (; i < l; i++) {

				levels[i].object->visible = false;

			}

		}
	}
}
