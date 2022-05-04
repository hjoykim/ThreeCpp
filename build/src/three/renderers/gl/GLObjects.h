#ifndef GL_OBJECTS_THREE_H
#define GL_OBJECTS_THREE_H
#include <three/renderers/gl/GLInfo.h>
#include <three/renderers/gl/GLAttributes.h>
#include <three/renderers/gl/GLGeometries.h>
#include <three/objects/InstancedMesh.h>
namespace three {
	namespace gl {
		class GLObjects {
		public:
			using ptr = std::shared_ptr<GLObjects>;
			std::unordered_map<unsigned, unsigned> updateList;

			GLInfo::ptr info;

			GLAttributes::ptr attributes;

			GLGeometries::ptr geometries;

			GLObjects(GLGeometries::ptr& geometries,GLAttributes::ptr& attributes, GLInfo::ptr& info) : geometries(geometries),attributes(attributes),info(info) {}

			virtual ~GLObjects() = default;

			GLObjects& copy(const GLObjects& source) {
				updateList = source.updateList;
				attributes = source.attributes;
				geometries = source.geometries;
				info = source.info;

				return *this;
			}
			GLObjects& operator = (const GLObjects& source) {
				return copy(source);
			}

			std::shared_ptr<BufferGeometry> update(Object3D& object);
		};		
		
	}
}
#endif