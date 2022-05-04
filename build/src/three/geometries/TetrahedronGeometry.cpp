#include "TetrahedronGeometry.h"
namespace three {
	namespace tetrahedron_geometry {
		std::vector<float> tet_vertices =
		{
			1, 1, 1, -1, -1, 1, -1, 1, -1, 1, -1, -1
		};

		std::vector<unsigned> tet_indices = 
		{
			2, 1, 0, 0, 3, 2, 1, 3, 0, 2, 3, 1
		};
	}
	using namespace tetrahedron_geometry;
	TetrahedronBufferGeometry::TetrahedronBufferGeometry(float radius, float detail) : PolyhedronBufferGeometry(tet_vertices,tet_indices,radius,detail)
	{
	}
	TetrahedronGeometry::TetrahedronGeometry(float radius, float detail) : Geometry()
	{
		
		TetrahedronBufferGeometry::ptr bufferGeometry = std::make_shared<TetrahedronBufferGeometry>(radius, detail);

		fromBufferGeometry(*bufferGeometry);
		mergeVertices();
	}
}
