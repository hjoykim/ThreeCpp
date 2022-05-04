#ifndef PLANE_GEOMETRY_THREE_H
#define PLANE_GEOMETRY_THREE_H
#include <three/core/Geometry.h>
#include <three/core/BufferGeometry.h>
namespace three {
	
    struct PlaneParameter {
        float width;
        float height;
        float widthSegments;
        float heightSegments;
    };
	class PlaneBufferGeometry : public BufferGeometry {
    protected:
        float width;
        float height;
        float widthSegments;
        float heightSegments;
	public:

		using ptr = std::shared_ptr<PlaneBufferGeometry>;
        PlaneParameter parameters;

        PlaneBufferGeometry(float width = 0, float height = 0, float widthSegments = 0, float heightSegments = 0);

		static ptr create(float width=0, float height=0, float widthSegments = 0, float heightSegments = 0) {
			return std::make_shared<PlaneBufferGeometry>(width, height, widthSegments, heightSegments);
		}
	};

    class PlaneGeometry : public Geometry {
    protected:
        float width;
        float height;
        float widthSegments;
        float heightSegments;
    public:

        using ptr = std::shared_ptr<PlaneGeometry>;
        PlaneParameter parameters;
        PlaneGeometry(float width = 0, float height = 0, float widthSegments = 0, float heightSegments = 0);

        static ptr create(float width = 0, float height = 0, float widthSegments = 0, float heightSegments = 0) {
            return std::make_shared<PlaneGeometry>(width, height, widthSegments, heightSegments);
        }

    };
}
#endif
