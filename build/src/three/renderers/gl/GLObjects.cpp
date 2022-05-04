#include "GLObjects.h"
namespace three {
	namespace gl {
		std::shared_ptr<BufferGeometry> three::gl::GLObjects::update(Object3D& object)
		{
			auto frame = info->render.frame;
			auto geometry = object.geometry;
			auto bufferGeometry = geometries->get(object, geometry);

			if (updateList.count(bufferGeometry->id) == 0 || updateList[bufferGeometry->id] != frame) {
				if (geometry->isGeometry) {
					bufferGeometry->updateFromObject(object);
				}
				geometries->update(bufferGeometry);

				if (updateList.count(bufferGeometry->id) == 0)
					updateList.insert({ bufferGeometry->id,frame });
				else {
					updateList[bufferGeometry->id] = frame;
				}
			}

			if (object.isInstancedMesh) {
				InstancedMesh* instanceMesh = static_cast<InstancedMesh*>(&object);
				attributes->update<float>(instanceMesh->instanceMatrix, GL_ARRAY_BUFFER);
			}

			return bufferGeometry;
		}
	}
}
