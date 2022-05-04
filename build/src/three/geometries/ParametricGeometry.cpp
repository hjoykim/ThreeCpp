#include "ParametricGeometry.h"
namespace three {
	ParametricBufferGeometry::ParametricBufferGeometry(std::function<Vector3& (float, float, Vector3&)> func, int slices, float stacks) : BufferGeometry()
	{
		this->func= func;
		this->slices=slices;
		this->stacks=stacks;

		std::vector<unsigned> _indices;
		std::vector<float> _vertices;
		std::vector<float> _normals;
		std::vector<float> _uvs;;

		float EPS = 0.00001f;

		Vector3 normal ;

		Vector3 p0 ;
		Vector3 p1 ;
		Vector3 pu ;
		Vector3 pv ;

		int i, j;

		// generate vertices, normals and uvs

		int sliceCount = slices + 1;

		for (i = 0; i <= stacks; i++)
		{

			auto v = i / stacks;

			for (j = 0; j <= slices; j++)
			{

				auto u = j / (float)slices;

				// vertex

				func(u, v, p0);
				_vertices.push_back(p0.x);
				_vertices.push_back(p0.y);
				_vertices.push_back(p0.z);

				// normal

				// approximate tangent vectors via finite differences

				if (u - EPS >= 0)
				{

					func(u - EPS, v, p1);
					pu.subVectors(p0, p1);

				}
				else
				{

					func(u + EPS, v, p1);
					pu.subVectors(p1, p0);

				}

				if (v - EPS >= 0)
				{

					func(u, v - EPS, p1);
					pv.subVectors(p0, p1);

				}
				else
				{

					func(u, v + EPS, p1);
					pv.subVectors(p1, p0);

				}

				// cross product of tangent vectors returns surface normal

				normal.crossVectors(pu, pv);
				normal.normalize();
				_normals.push_back(normal.x);
				_normals.push_back(normal.y);
				_normals.push_back(normal.z);

				// uv

				_uvs.push_back(u);
				_uvs.push_back(v);

			}

		}

		// generate indices

		for (i = 0; i < stacks; i++)
		{

			for (j = 0; j < slices; j++)
			{

				int a = i * sliceCount + j;
				int b = i * sliceCount + j + 1;
				int c = (i + 1) * sliceCount + j + 1;
				int d = (i + 1) * sliceCount + j;

				// faces one and two
				_indices.push_back(a); _indices.push_back(b); _indices.push_back(d);
				_indices.push_back(b); _indices.push_back(c); _indices.push_back(d);


			}

		}

		// build geometry

		//setIndex("index",_indices);
		this->index = BufferAttribute<unsigned>::create(_indices, 1);
		setAttribute(AttributeName::position,BufferAttribute<float>::create(_vertices, 3));
		setAttribute(AttributeName::normal,BufferAttribute<float>::create(_normals, 3));
		setAttribute(AttributeName::uv,BufferAttribute<float>::create(_uvs, 2));
	}
	ParametricGeometry::ParametricGeometry(std::function<Vector3& (float, float, Vector3&)> func, int slices, float stacks)
	{
		this->func = func;
		this->slices = slices;
		this->stacks = stacks;

		ParametricBufferGeometry::ptr bufferGeometry = std::make_shared<ParametricBufferGeometry>(func, slices, stacks);
		fromBufferGeometry(*bufferGeometry);
		mergeVertices();

	}
}
