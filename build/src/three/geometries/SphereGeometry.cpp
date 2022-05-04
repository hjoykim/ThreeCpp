#include "SphereGeometry.h"

three::SphereBufferGeometry::SphereBufferGeometry(float radius, float widthSegments, float heightSegments, float phiStart, float phiLength, float thetaStart, float thetaLength) : BufferGeometry()
{


	radius = radius != 0 ? radius : 1;

	if (widthSegments == 0) widthSegments = 8;
	if (heightSegments == 0) heightSegments = 6;

	widthSegments = std::max(3.0f, floor(widthSegments));

	heightSegments = std::max(2.0f, floor(heightSegments));

	phiStart = !std::isnan(phiStart) ? phiStart : 0;
	phiLength = !std::isnan(phiLength) ? phiLength : 2 * (float)M_PI; ;
	thetaStart = !std::isnan(thetaStart) ? thetaStart : 0;
	thetaLength = !std::isnan(thetaLength) ? thetaLength : (float)M_PI;
	

	auto thetaEnd = std::min(thetaStart + thetaLength, (float)M_PI);

	std::vector<unsigned> _indices;
	std::vector<float> _vertices;
	std::vector<float> _normals;
	std::vector<float> _uvs;
	std::vector<std::vector<unsigned>> _grid;

	int index = 0;

	Vector3 vertex;
	Vector3 normal;

	for (int iy = 0; iy <= heightSegments; iy++)
	{

		std::vector<unsigned> _verticesRow;

		float v = iy / heightSegments;

		// special case for the poles

		auto uOffset = 0.0f;

		if (iy == 0 && thetaStart == 0) {

			uOffset = 0.5f / widthSegments;

		}
		else if (iy == heightSegments && thetaEnd == (float)M_PI) {

			uOffset = -0.5f / widthSegments;

		}

		for (int ix = 0; ix <= widthSegments; ix++)
		{

			float u = ix / widthSegments;

			// vertex

			vertex.x = (-radius * cos(phiStart + u * phiLength) * sin(thetaStart + v * thetaLength));
			vertex.y = (radius * cos(thetaStart + v * thetaLength));
			vertex.z = (radius * sin(phiStart + u * phiLength) * sin(thetaStart + v * thetaLength));

			_vertices.push_back(vertex.x);
			_vertices.push_back(vertex.y);
			_vertices.push_back(vertex.z);

			// normal

			normal.copy(vertex).normalize();
			_normals.push_back(normal.x);
			_normals.push_back(normal.y);
			_normals.push_back(normal.z);

			// uv

			_uvs.push_back(u + uOffset);
			_uvs.push_back(1 - v);

			_verticesRow.push_back(index++);

		}

		_grid.push_back(_verticesRow);
	}

	// _indices

	for (int iy = 0; iy < (int)heightSegments; iy++) {

		for (int ix = 0; ix < (int)widthSegments; ix++) {

			auto a = _grid[iy][ix + 1];
			auto b = _grid[iy][ix];
			auto c = _grid[iy + 1][ix];
			auto d = _grid[iy + 1][ix + 1];

			if (iy != 0 || thetaStart > 0) {
				_indices.push_back(a);
				_indices.push_back(b);
				_indices.push_back(d);
			}
			if (iy != (int)(heightSegments - 1) || thetaEnd < M_PI) {
				_indices.push_back(b);
				_indices.push_back(c);
				_indices.push_back(d);
			}
		}
	}

	// build geometry
	this->index = BufferAttribute<unsigned>::create(_indices, 1);
	setAttribute(AttributeName::position,BufferAttribute<float>::create(_vertices, 3));
	setAttribute(AttributeName::normal,BufferAttribute<float>::create(_normals, 3));
	setAttribute(AttributeName::uv, BufferAttribute<float>::create(_uvs, 2));
}

three::SphereGeometry::SphereGeometry(float radius, float widthSegments, float heightSegments, float phiStart, float phiLength, float thetaStart, float thetaLength) : Geometry()
{
	
	SphereBufferGeometry::ptr bufferGeometry = SphereBufferGeometry::create(radius, widthSegments, heightSegments, phiStart, phiLength, thetaStart, thetaLength);

	this->fromBufferGeometry(*bufferGeometry);
	this->mergeVertices();
}
