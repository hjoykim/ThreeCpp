#ifndef GL_GEOMETRIES_THREE_H
#define GL_GEOMETRIES_THREE_H
#include <three/core/Object3D.h>
#include <three/core/Geometry.h>
#include <three/core/BufferGeometry.h>
#include <three/core/BufferAttribute.h>
#include <three/renderers/gl/GLInfo.h>
#include <three/renderers/gl/GLAttributes.h>
namespace three {
	namespace gl {		
		class GLGeometries {
		private:
			struct GeometryInfo {
				BufferGeometry::ptr bufferGeometry;
				BufferGeometry::OnDispose::ConnectionId connectionId;
			};

			std::unordered_map<unsigned,GeometryInfo> geometries;

			std::unordered_map<unsigned,BufferAttribute<unsigned>::ptr>wireframeAttributes;

			GLAttributes::ptr attributes;

			GLInfo::ptr info;

			void onGeometryDispose(Geometry* geometry);

			void updateWireframeAttribute(BufferGeometry& geometry);

		public:
			using ptr = std::shared_ptr<GLGeometries>;


			GLGeometries(GLAttributes::ptr& attributes, GLInfo::ptr& info) : attributes(attributes), info(info) {}

			std::shared_ptr<BufferGeometry> get(Object3D& object, std::shared_ptr<Geometry>& geometry);

			void update(const std::shared_ptr<BufferGeometry>& geometry);			
			
			std::shared_ptr<BufferAttribute<unsigned>> getWireframeAttribute(const std::shared_ptr<BufferGeometry>& geometry);
			

			GLGeometries& copy(const GLGeometries& source) {
				attributes = source.attributes;
				info = source.info;
				geometries = source.geometries;
				wireframeAttributes = source.wireframeAttributes;
				return *this;
			}
			GLGeometries& operator = (const GLGeometries& source) {
				return copy(source);
			}
		};
		
		
	}
}
#endif