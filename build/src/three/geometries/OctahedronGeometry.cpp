#include "OctahedronGeometry.h"
namespace three {
	namespace octahedron_geometry {
        std::vector<float> oct_vertices = 
        {
            1, 0, 0, -1, 0, 0, 0, 1, 0,
            0, -1, 0, 0, 0, 1, 0, 0, -1
        };

        std::vector<unsigned> oct_indices = 
        {
            0, 2, 4, 0, 4, 3, 0, 3, 5,
            0, 5, 2, 1, 2, 5, 1, 5, 3,
            1, 3, 4, 1, 4, 2
        };
	}
	using namespace octahedron_geometry;
	OctahedronBufferGeometry::OctahedronBufferGeometry(float radius, float detail) : PolyhedronBufferGeometry(oct_vertices,oct_indices,radius, detail)
	{
	}
    OctahedronGeometry::OctahedronGeometry(float radius, float detail)
    {
        this->radius= radius;
        this->detail= detail;

        OctahedronBufferGeometry::ptr bufferGeometry = std::make_shared<OctahedronBufferGeometry>(radius, detail);

        fromBufferGeometry(*bufferGeometry);
        mergeVertices();
    }
}
