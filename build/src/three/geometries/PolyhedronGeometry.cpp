#include "PolyhedronGeometry.h"
namespace three {
    void PolyhedronBufferGeometry::getVertexByIndex(unsigned index, Vector3& vertex)
    {
        auto stride = index * 3;

        vertex.x = _vertices[stride + 0];
        vertex.y = _vertices[stride + 1];
        vertex.z = _vertices[stride + 2];
    }
	void PolyhedronBufferGeometry::pushVertex(const Vector3& vertex)
	{
		_vertexBuffer.push_back(vertex.x);
		_vertexBuffer.push_back(vertex.y);
		_vertexBuffer.push_back(vertex.z);
	}
    void PolyhedronBufferGeometry::subdivide(float detail)
	{
        Vector3 a;
        Vector3 b;
        Vector3 c;

        // iterate over all faces and apply a subdivison with the given detail value

        for (unsigned i = 0; i < _indices.size(); i += 3)
        {

            // get the vertices of the face

            getVertexByIndex(_indices[i + 0], a);
            getVertexByIndex(_indices[i + 1], b);
            getVertexByIndex(_indices[i + 2], c);

            // perform subdivision

            subdivideFace(a, b, c, detail);

        }
	}
    void PolyhedronBufferGeometry::subdivideFace(const Vector3& a, const Vector3& b, const Vector3& c, float detail)
    {
		float cols = (float)pow(2, detail);

		// we use this multidimensional array as a data structure for creating the subdivision
		std::vector<std::vector<Vector3>> v;

		int i, j;

		// construct all of the vertices for this subdivision

		for (i = 0; i <= cols; i++)
		{
			v.push_back(std::vector<Vector3>());

			Vector3 aj, bj;
			aj.copy(a).lerp(c, i / cols);
			bj.copy(b).lerp(c, i / cols);

			float rows = cols - i;

			for (j = 0; j <= rows; j++)
			{

				if (j == 0 && i == cols)
				{

					v[i].push_back(aj);

				}
				else
				{
					Vector3 cj;
					v[i].push_back(cj.copy(aj).lerp(bj, j / rows));

				}

			}

		}

		// construct all of the faces

		for (i = 0; i < cols; i++)
		{

			for (j = 0; j < 2 * (cols - i) - 1; j++)
			{

				int k = (int)floor(j / 2);

				if (j % 2 == 0)
				{

					pushVertex(v[i][k + 1]);
					pushVertex(v[i + 1][k]);
					pushVertex(v[i][k]);

				}
				else
				{

					pushVertex(v[i][k + 1]);
					pushVertex(v[i + 1][k + 1]);
					pushVertex(v[i + 1][k]);

				}

			}

		}
    }
	void PolyhedronBufferGeometry::applyRadius(float radius)
	{
		Vector3 vertex;

		// iterate over the entire buffer and apply the radius to each vertex

		for (auto i = 0; i < _vertexBuffer.size(); i += 3) {

			vertex.x = _vertexBuffer[i + 0];
			vertex.y = _vertexBuffer[i + 1];
			vertex.z = _vertexBuffer[i + 2];

			vertex.normalize().multiplyScalar(radius);

			_vertexBuffer[i + 0] = vertex.x;
			_vertexBuffer[i + 1] = vertex.y;
			_vertexBuffer[i + 2] = vertex.z;

		}

	}
	void PolyhedronBufferGeometry::generateUVs()
	{
		Vector3 vertex;

		for (auto i = 0; i < _vertexBuffer.size(); i += 3) {

			vertex.x = _vertexBuffer[i + 0];
			vertex.y = _vertexBuffer[i + 1];
			vertex.z = _vertexBuffer[i + 2];

			auto u = azimuth(vertex) / 2 / (float)M_PI + 0.5f;
			auto v = inclination(vertex) / (float)M_PI + 0.5f;
			_uvBuffer.push_back(u);
			_uvBuffer.push_back(1 - v);

		}

		correctUVs();

		correctSeam();
	}
	void PolyhedronBufferGeometry::correctSeam()
	{
		for (auto i = 0; i < _uvBuffer.size(); i += 6)
		{

			// uv data of a single face

			auto x0 = _uvBuffer[i + 0];
			auto x1 = _uvBuffer[i + 2];
			auto x2 = _uvBuffer[i + 4];

			auto max = std::max(x0, std::max(x1, x2));
			auto min = std::min(x0, std::min(x1, x2));

			// 0.9 is somewhat arbitrary

			if (max > 0.9f && min < 0.1f)
			{

				if (x0 < 0.2f) _uvBuffer[i + 0] += 1;
				if (x1 < 0.2f) _uvBuffer[i + 2] += 1;
				if (x2 < 0.2f) _uvBuffer[i + 4] += 1;

			}

		}
	}
	void PolyhedronBufferGeometry::correctUVs()
	{
		Vector3 a,b,c,centroid;


		Vector2 uvA,uvB,uvC;

		for (auto i = 0, j = 0; i < _vertexBuffer.size(); i += 9, j += 6) {

			a.set(_vertexBuffer[i + 0], _vertexBuffer[i + 1], _vertexBuffer[i + 2]);
			b.set(_vertexBuffer[i + 3], _vertexBuffer[i + 4], _vertexBuffer[i + 5]);
			c.set(_vertexBuffer[i + 6], _vertexBuffer[i + 7], _vertexBuffer[i + 8]);

			uvA.set(_uvBuffer[j + 0], _uvBuffer[j + 1]);
			uvB.set(_uvBuffer[j + 2], _uvBuffer[j + 3]);
			uvC.set(_uvBuffer[j + 4], _uvBuffer[j + 5]);

			centroid.copy(a).add(b).add(c).divideScalar(3);

			float azi = azimuth(centroid);

			correctUV(uvA, j + 0, a, azi);
			correctUV(uvB, j + 2, b, azi);
			correctUV(uvC, j + 4, c, azi);

		}

	}
	void PolyhedronBufferGeometry::correctUV(const Vector2& uv, int stride, const Vector3& vector, float azimuth)
	{
		if ((azimuth < 0) && (uv.x ==  1)) {

			_uvBuffer[stride] = uv.x - 1;

		}

		if ((vector.x ==  0) && (vector.z ==  0)) {

			_uvBuffer[stride] = azimuth / 2 / (float)M_PI + 0.5f;

		}
	}
	float PolyhedronBufferGeometry::azimuth(const Vector3& vector)
	{
		return atan2(vector.z, -vector.x);
	}
	float PolyhedronBufferGeometry::inclination(const Vector3& vector)
	{
		return atan2(-vector.y, sqrt((vector.x * vector.x) + (vector.z * vector.z)));
	}
	
	PolyhedronBufferGeometry::PolyhedronBufferGeometry(const std::vector<float>& vertices, const std::vector<unsigned>& indices, float radius, float detail)
	{	

		radius = radius ? radius : 1.0f;
		detail = !std::isnan(detail) ? detail : 0;
		_vertices = vertices;
		_indices = indices;
		subdivide(detail);
		applyRadius(radius);
		generateUVs();

		setAttribute(AttributeName::position,BufferAttribute<float>::create(_vertexBuffer, 3));
		setAttribute(AttributeName::normal,BufferAttribute<float>::create(_vertexBuffer, 3));
		setAttribute(AttributeName::uv,BufferAttribute<float>::create(_uvBuffer, 2));

		if (detail == 0) {
			computeVertexNormals();
		}
		else {
			normalizeNormals();
		}
	}
	PolyhedronGeometry::PolyhedronGeometry(const std::vector<float>& vertices, const std::vector<unsigned>& indices, float radius, float detail) :Geometry()
	{
		auto polyBufferGeometry = PolyhedronBufferGeometry::create(vertices, indices, radius, detail);
		//auto directGeometry = dynamic_pointer_cast<DirectGeometry::ptr>(geometry);
		//auto bufferGeometry = dynamic_pointer_cast<BufferGeometry::ptr>(polyBufferGeometry);

		this->fromBufferGeometry(*polyBufferGeometry);
		this->mergeVertices();
	}
}
