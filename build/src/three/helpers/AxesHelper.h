#ifndef AXES_HELPER_THREE_H
#define AXES_HELPER_THREE_H
#include <three/objects/Line.h>
#include <three/materials/LineBasicMaterial.h>
#include <three/core/BufferAttribute.h>
#include <three/core/BufferGeometry.h>
#include <three/Constants.h>
namespace three {
	class AxesHelper : public LineSegments {
	public:
        using ptr = std::shared_ptr<AxesHelper>;
		AxesHelper(unsigned size) : LineSegments() {
            float vertices[] =
            {
                0,0,0,size,0,0,
                0,0,0,0,size,0,
                0,0,0,0,0,size
            };

            float colors[] =
            {
                1,0,0, 1,0.6f,0,
                0,1,0, 0.6f,1,0,
                0,0,0,  0,0.6f,1
            };

            BufferGeometry::ptr bufferGeometry = BufferGeometry::create();
            bufferGeometry->setAttribute(AttributeName::position,BufferAttribute<float>::create(vertices, 18, 3));
            bufferGeometry->setAttribute(AttributeName::color,BufferAttribute<float>::create(colors, 18, 3));           

            LineBasicMaterial::ptr material = std::make_shared<LineBasicMaterial>();
           
            material->vertexColors = true;
            this->geometry = bufferGeometry;
            this->material = material;
		}

        static ptr create(unsigned size) {
            return std::make_shared<AxesHelper>(size);
        }
		
	};
}
#endif
