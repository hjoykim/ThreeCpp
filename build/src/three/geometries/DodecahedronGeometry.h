#ifndef DODECAHEDRON_GEOMETRY_THREE_H
#define  DODECAHEDRON_GEOMETRY_THREE_H
#include <three/geometries/PolyhedronGeometry.h>
namespace three {	

	class DodecahedronBufferGeometry : public PolyhedronBufferGeometry {		
	public:
		float radius;
		float detail;
		using ptr = std::shared_ptr<DodecahedronBufferGeometry>;
		DodecahedronBufferGeometry(float radius = 0, float detail = 0);
		
		

		static const ptr create(float radius = 0, float detail = 0) {
			return std::make_shared<DodecahedronBufferGeometry>(radius, detail);
		}
	};

	class DodecahedronGeometry : public Geometry {
	public:
		using ptr = std::shared_ptr<DodecahedronGeometry>;
		float radius;
		float detail;
		DodecahedronGeometry(float radius = 0, float detail = 0);	
		
		static const ptr create(float radius = 0, float detail = 0) {
			return std::make_shared<DodecahedronGeometry>(radius, detail);
		}
	};
}
#endif
