#include "TorusKnotGeometry.h"
namespace three {
	void TorusKnotBufferGeometry::calculatePositionOnCurve(float u, float p, float q, float radius, Vector3& position)
	{
		auto cu = (float)cos(u);
		auto su = (float)sin(u);
		auto quOverP = q / p * u;
		auto cs = (float)cos(quOverP);

		position.x = radius * (2 + cs) * 0.5f * cu;
		position.y = radius * (2 + cs) * su * 0.5f;
		position.z = radius * (float)sin(quOverP) * 0.5f;
	}
	TorusKnotBufferGeometry::TorusKnotBufferGeometry(float radius, float tube, float tubularSegments, float radialSegments, float p, float q)
	{
		radius = radius != 0 ? radius : 1;
		tube = tube != 0 ? tube : 0.4f;
		radialSegments = radialSegments != 0 ? floor(radialSegments) : 64;
		tubularSegments = tubularSegments != 0 ? floor(tubularSegments) : 8;
		p = p != 0 ? p : 2;
		q = q != 0 ? q : 3;
				

		int i, j;

		std::vector<unsigned> _indices;
		std::vector<float> _vertices;
		std::vector<float> _normals;
		std::vector<float> _uvs;

		Vector3 vertex ;
		Vector3 normal ;

		Vector3 P1 ;
		Vector3 P2 ;

		Vector3 B ;
		Vector3 T ;
		Vector3 N ;
		// generate vertices, normals and uvs

		for (i = 0; i <= tubularSegments; ++i)
		{

			// the radian "u" is used to calculate the position on the torus curve of the current tubular segement

			float u = i / (float)tubularSegments * (float)p * (float)M_PI * 2;

			// now we calculate two points. P1 is our current position on the curve, P2 is a little farther ahead.
			// these points are used to create a special "coordinate space", which is necessary to calculate the correct vertex positions

			calculatePositionOnCurve(u, (float)p, (float)q, (float)radius, P1);
			calculatePositionOnCurve(u + 0.01f, (float)p, (float)q, (float)radius, P2);

			// calculate orthonormal basis

			T.subVectors(P2, P1);
			N.addVectors(P2, P1);
			B.crossVectors(T, N);
			N.crossVectors(B, T);

			// normalize B, N. T can be ignored, we don't use it

			B.normalize();
			N.normalize();

			for (j = 0; j <= radialSegments; ++j)
			{

				// now calculate the vertices. they are nothing more than an extrusion of the torus curve.
				// because we extrude a shape in the xy-plane, there is no need to calculate a z-value.

				auto v = (float)(j / radialSegments * (float)M_PI * 2);
				auto cx = (float)(-tube * cos(v));
				auto cy = (float)(tube * sin(v));

				// now calculate the final vertex position.
				// first we orient the extrusion with our basis vectos, then we add it to the current position on the curve

				vertex.x = P1.x + (cx * N.x + cy * B.x);
				vertex.y = P1.y + (cx * N.y + cy * B.y);
				vertex.z = P1.z + (cx * N.z + cy * B.z);

				_vertices.push_back(vertex.x);
				_vertices.push_back(vertex.y);
				_vertices.push_back(vertex.z);

				// normal (P1 is always the center/origin of the extrusion, thus we can use it to calculate the normal)

				normal.subVectors(vertex, P1).normalize();

				_normals.push_back(normal.x);
				_normals.push_back(normal.y);
				_normals.push_back(normal.z);

				// uv

				_uvs.push_back(i / (float)tubularSegments);
				_uvs.push_back(j / (float)radialSegments);

			}

		}

		// generate indices

		for (j = 1; j <= tubularSegments; j++)
		{

			for (i = 1; i <= radialSegments; i++)
			{

				// indices

				auto a = ((int)radialSegments + 1) * (j - 1) + (i - 1);
				auto b = ((int)radialSegments + 1) * j + (i - 1);
				auto c = ((int)radialSegments + 1) * j + i;
				auto d = ((int)radialSegments + 1) * (j - 1) + i;

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

		// this function calculates the current position on the torus curve


	}
	TorusKnotGeometry::TorusKnotGeometry(float radius, float tube, float tubularSegments, float radialSegments, float p, float q)
	{
	/*	parameters.add("radius", radius);
		parameters.add("tube", tube);
		parameters.add("radialSegments", radialSegments);
		parameters.add("tubularSegments", tubularSegments);
		parameters.add("p", p);
		parameters.add("q", q);*/

		TorusKnotBufferGeometry::ptr bufferGeometry = std::make_shared<TorusKnotBufferGeometry>(radius, tube, tubularSegments, radialSegments, p, q);

		fromBufferGeometry(*bufferGeometry);
		mergeVertices();
	}
}
