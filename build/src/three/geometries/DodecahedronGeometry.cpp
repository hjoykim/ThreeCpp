#include "DodecahedronGeometry.h"

namespace dodecahedron_buffergeometry {
	float t = (1.0 + std::sqrt(5)) / 2.0;

	float r = 1 / t;

	std::vector<float>  _vertices =
	{
		// (¡¾1, ¡¾1, ¡¾1)
		-1, -1, -1, -1, -1, 1,
		-1, 1, -1, -1, 1, 1,
		1, -1, -1, 1, -1, 1,
		1, 1, -1, 1, 1, 1,

		// (0, ¡¾1/¥õ, ¡¾¥õ)
		0, -r, -t, 0, -r, t,
		0, r, -t, 0, r, t,

		// (¡¾1/¥õ, ¡¾¥õ, 0)
		-r, -t, 0, -r, t, 0,
		 r, -t, 0, r, t, 0,

		 // (¡¾¥õ, 0, ¡¾1/¥õ)
		 -t, 0, -r, t, 0, -r,
		 -t, 0, r, t, 0, r
	};

	std::vector<unsigned> _indices =
	{
		3, 11, 7, 3, 7, 15, 3, 15, 13,
		7, 19, 17, 7, 17, 6, 7, 6, 15,
		17, 4, 8, 17, 8, 10, 17, 10, 6,
		8, 0, 16, 8, 16, 2, 8, 2, 10,
		0, 12, 1, 0, 1, 18, 0, 18, 16,
		6, 10, 2, 6, 2, 13, 6, 13, 15,
		2, 16, 18, 2, 18, 3, 2, 3, 13,
		18, 1, 9, 18, 9, 11, 18, 11, 3,
		4, 14, 12, 4, 12, 0, 4, 0, 8,
		11, 9, 5, 11, 5, 19, 11, 19, 7,
		19, 5, 14, 19, 14, 4, 19, 4, 17,
		1, 12, 14, 1, 14, 5, 1, 5, 9
	};
}
using namespace dodecahedron_buffergeometry;

three::DodecahedronBufferGeometry::DodecahedronBufferGeometry(float radius, float detail) : PolyhedronBufferGeometry(dodecahedron_buffergeometry::_vertices, dodecahedron_buffergeometry::_indices, radius, detail)
{	
}

three::DodecahedronGeometry::DodecahedronGeometry(float radius, float detail) : Geometry()
{
	this->radius = radius;
	this->detail = detail;

	auto dodecahedronBufferGeometry = DodecahedronBufferGeometry::create(radius, detail);

	//auto bufferGeometry = dynamic_pointer_cast<BufferGeometry::ptr>(dodecahedronBufferGeometry);

	this->fromBufferGeometry(*dodecahedronBufferGeometry);
	this->mergeVertices();
}
