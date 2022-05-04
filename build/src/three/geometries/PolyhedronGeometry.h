#ifndef POLYHEDRONBUFFER_GEOMETRY_THREE_H
#define POLYHEDRONBUFFER_GEOMETRY_THREE_H
#include <three/core/BufferGeometry.h>
#include <memory>
#include <vector>
namespace three {	

	class PolyhedronBufferGeometry : public BufferGeometry {

	private :
		std::vector<float> _vertices;
		std::vector<unsigned> _indices;
		std::vector<float> _vertexBuffer;
		std::vector<float> _uvBuffer;

		void getVertexByIndex(unsigned index, Vector3& vertex);

		void pushVertex(const Vector3& vertex);

		void subdivide(float detail);

		void subdivideFace(const Vector3& a,const Vector3& b, const Vector3& c, float detail);


		void applyRadius(float radius);

		void generateUVs();

		void correctSeam();

		void correctUVs();

		void correctUV(const Vector2& uv, int stride, const Vector3& vector, float azimuth);

		float azimuth(const Vector3& vector);

		float inclination(const Vector3& vector);


	public:
		using ptr = std::shared_ptr<PolyhedronBufferGeometry>;
		PolyhedronBufferGeometry(const std::vector<float>& vertices, const std::vector<unsigned>& indices, float radius = 0, float detail = 0);	

		static ptr create(const std::vector<float>& vertices, const std::vector<unsigned>& indices, float radius = 0, float detail = 0) {
			return std::make_shared<PolyhedronBufferGeometry>(vertices, indices, radius, detail);
		}
	};

	class PolyhedronGeometry : public Geometry {
	public:
		PolyhedronGeometry(const std::vector<float>& vertices, const std::vector<unsigned>& indices, float radius = 0, float detail = 0);
		
		using ptr = std::shared_ptr<PolyhedronGeometry>;

		static ptr create(const std::vector<float>& vertices, const std::vector<unsigned>& indices, float radius = 0, float detail = 0) {
			return std::make_shared<PolyhedronGeometry>(vertices, indices, radius, detail);
		}
	};
}
#endif
