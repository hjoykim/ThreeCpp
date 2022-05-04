#ifndef LOD_THREE_H
#define LOD_THREE_H
#include <three/core/Object3D.h>
namespace three {
	class Camera;
	struct LevelStruct {
		using ptr = std::shared_ptr<LevelStruct>;
		float distance;
		Object3D::ptr object;
		LevelStruct() {}
		LevelStruct(const Object3D::ptr& object, float distance) : object(object), distance(distance) {}
	};
	class LOD : public Object3D {
	private:
		int _currentLevel = 0;
		Vector3 _v1;
		Vector3 _v2;
	public:
		using ptr = std::shared_ptr<LOD>;

		bool autoUpdate = true;

		std::vector<LevelStruct> levels;

		LOD() : Object3D() {
			
		}
		
		LOD(const LOD& source) : Object3D(source) {
			autoUpdate = source.autoUpdate;

			if (source.levels.size() > 0) {
				levels.resize(source.levels.size());
				std::copy(source.levels.begin(), source.levels.end(), levels.begin());
			}
		}

		LOD* clone() {
			return new LOD(*this);
		}

		LOD& copy(const LOD& source) {

			Object3D::copy(source);
			autoUpdate = source.autoUpdate;

			if (source.levels.size() > 0) {
				levels.resize(source.levels.size());
				std::copy(source.levels.begin(), source.levels.end(), levels.begin());
			}
			return *this;
		}

		LOD& addLevel(const Object3D::ptr& object, float distance = 0);

		int getCurrentLevel() {
			return _currentLevel;
		}

		std::shared_ptr<Object3D> getObjectForDistance(float distance);

		void raycast() {}

		void update(const Camera& camera);
	};
}
#endif