#ifndef FACE3_THREE_H
#define FACE3_THREE_H
#include <three/math/Vector3.h>
#include <three/math/Color.h>
#include <vector>
using namespace three::math;
namespace three {
	class Face3 {
		static size_t _faceId;
	public:
		union {
			struct { unsigned a, b, c; };
			unsigned elements[3];
		};

		size_t _id;

		Vector3 normal;

		std::vector<Vector3> vertexNormals;

		Color color;

		std::vector<Color> vertexColors;

		unsigned materialIndex;

		Face3(); 
		

		Face3(unsigned a, unsigned b, unsigned c); 
		
		

		Face3(unsigned a, unsigned b, unsigned c, Vector3 normal, Color color, unsigned materialIndex=0);
				
		Face3(unsigned a, unsigned b, unsigned c, std::vector<Vector3> * normal, Color* color, unsigned materialIndex = 0);

		Face3(unsigned a, unsigned b, unsigned c, std::vector<Vector3>* normal, std::vector<Color> * colors, unsigned materialIndex = 0);

		Face3(const Face3& source);

		Face3& clone(Face3* target);

		Face3& copy(Face3& source);

		Vector3 __originalFaceNormal;
		std::vector<Vector3> __originalVertexNormals;

		
	};
}
#endif