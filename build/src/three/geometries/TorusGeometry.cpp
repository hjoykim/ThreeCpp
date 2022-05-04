#include "TorusGeometry.h"
namespace three {
	TorusBufferGeometry::TorusBufferGeometry(float radius, float tube, float radialSegments, float tubularSegments, float arc) : BufferGeometry()
	{
		radius = radius != 0 ? radius : 1;
		tube = tube != 0 ? tube : 1;
		radialSegments = radialSegments != 0 ? floor(radialSegments) : 8;
		tubularSegments = tubularSegments != 0 ? floor(tubularSegments) : 6;
		arc = arc != 0 ? arc : (float)M_PI * 2;

	

		std::vector<unsigned> _indices;
		std::vector<float> _vertices;
		std::vector<float> _normals;
		std::vector<float> _uvs;

		// helper variables

		Vector3 center;
		Vector3 vertex;
		Vector3 normal;

		int j, i;

		// generate vertices, normals and uvs

		for (j = 0; j <= radialSegments; j++)
		{

			for (i = 0; i <= tubularSegments; i++)
			{

				float u = i / (float)tubularSegments * (float)arc;
				float v = j / (float)radialSegments * (float)M_PI * 2;

				// vertex

				vertex.x = (float)((radius + tube * cos(v)) * cos(u));
				vertex.y = (float)((radius + tube * cos(v)) * sin(u));
				vertex.z = (float)(tube * sin(v));

				_vertices.push_back(vertex.x);
				_vertices.push_back(vertex.y);
				_vertices.push_back(vertex.z);

				// normal

				center.x = radius * (float)cos(u);
				center.y = radius * (float)sin(u);
				normal.subVectors(vertex, center);
				normal.normalize();

				_normals.push_back(normal.x);
				_normals.push_back(normal.y);
				_normals.push_back(normal.z);

				// uv

				_uvs.push_back(i / tubularSegments);
				_uvs.push_back(j / radialSegments);

			}

		}

		// generate indices

		for (j = 1; j <= radialSegments; j++)
		{

			for (i = 1; i <= tubularSegments; i++)
			{

				// indices

				int a = ((int)tubularSegments + 1) * j + i - 1;
				int b = ((int)tubularSegments + 1) * (j - 1) + i - 1;
				int c = ((int)tubularSegments + 1) * (j - 1) + i;
				int d = ((int)tubularSegments + 1) * j + i;

				// faces

				_indices.push_back(a);
				_indices.push_back(b);
				_indices.push_back(d);

				_indices.push_back(b);
				_indices.push_back(c);
				_indices.push_back(d);

			}

		}

		// build geometry

		this->index = BufferAttribute<unsigned>::create(_indices, 1);
		setAttribute(AttributeName::position,BufferAttribute<float>::create(_vertices, 3));
		setAttribute(AttributeName::normal,BufferAttribute<float>::create(_normals, 3));
		setAttribute(AttributeName::uv,BufferAttribute<float>::create(_uvs, 2));
	}
	TorusGeometry::TorusGeometry(float radius, float tube, float radialSegments, float tubularSegments, float arc) : Geometry()
	{
	

		TorusBufferGeometry::ptr bufferGeometry = std::make_shared<TorusBufferGeometry>(radius, tube, radialSegments, tubularSegments, arc);

		fromBufferGeometry(*bufferGeometry);
		mergeVertices();
	}
}
