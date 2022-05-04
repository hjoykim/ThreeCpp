#include "LatheGeometry.h"
namespace three {
	LatheBufferGeometry::LatheBufferGeometry(const std::vector<Vector3>& points, float segments, float phiStart, float phiLength) : BufferGeometry()
	{
		segments = !std::isnan(segments) ? floor(segments) : 12;
		phiStart = !std::isnan(phiStart)? phiStart : 0;
		phiLength = !std::isnan(phiLength) ? phiLength : (float)M_PI * 2;
		phiLength = clamp(phiLength,0.0f, (float)M_PI * 2);

		this->points= points;
		this->segments= segments;
		this->phiStart= phiStart;
		this->phiLength= phiLength;

		std::vector<unsigned> _indices;
        std::vector<float> _vertices;
        std::vector<float> _uvs;
        float inverseSegments = 1.0f / (float)segments;

        Vector3 vertex;
        Vector2 uv;
        int i, j;

        // generate vertices and uvs

        for (i = 0; i <= segments; i++)
        {

            auto phi = phiStart + i * inverseSegments * phiLength;

            auto sin = (float)std::sin(phi);
            auto cos = (float)std::cos(phi);

            for (j = 0; j <= (points.size() - 1); j++)
            {

                // vertex

                vertex.x = points[j].x * sin;
                vertex.y = points[j].y;
                vertex.z = points[j].x * cos;

                _vertices.push_back(vertex.x); _vertices.push_back(vertex.y); _vertices.push_back(vertex.z);

                // uv

                uv.x = i / segments;
                uv.y = j / (float)(points.size() - 1);

                _uvs.push_back(uv.x); _uvs.push_back(uv.y);


            }

        }

        // indices

        for (i = 0; i < segments; i++)
        {

            for (j = 0; j < (points.size() - 1); j++)
            {

                int ba = j + i * points.size();

                auto a = ba;
                auto b = ba + points.size();
                auto c = ba + points.size() + 1;
                auto d = ba + 1;

                // faces

                _indices.push_back(a); _indices.push_back(b); _indices.push_back(d);
                _indices.push_back(b); _indices.push_back(c); _indices.push_back(d);

            }
        }

        index = BufferAttribute<unsigned>::create(_indices, 1);
        setAttribute(AttributeName::position,BufferAttribute<float>::create(_vertices, 3));
        setAttribute(AttributeName::uv,BufferAttribute<float>::create(_uvs, 2));

        computeVertexNormals();

        if (phiLength == (float)M_PI * 2)
        {

            BufferAttribute<float>::ptr normals = getAttribute(AttributeName::normal);
                //(this.Attributes["normal"] as BufferAttribute<float>).Array;

            Vector3 n1;
            Vector3 n2;
            Vector3 n;

            // this is the buffer offset for the last line of vertices

            int ba = (int)segments * points.size() * 3;

            for (i = 0, j = 0; i < points.size(); i++, j += 3)
            {

                // select the normal of the vertex in the first line

                n1.x = normals->array[j + 0];
                n1.y = normals->array[j + 1];
                n1.z = normals->array[j + 2];

                // select the normal of the vertex in the last line

                n2.x = normals->array[ba + j + 0];
                n2.y = normals->array[ba + j + 1];
                n2.z = normals->array[ba + j + 2];

                // average normals

                n.addVectors(n1, n2);
                n.normalize();

                // assign the new values to both normals

                normals->array[j + 0] = normals->array[ba + j + 0] = n.x;
                normals->array[j + 1] = normals->array[ba + j + 1] = n.y;
                normals->array[j + 2] = normals->array[ba + j + 2] = n.z;

            }
        }   
	}
    LatheGeometry::LatheGeometry(const std::vector<Vector3>& points, float segments, float phiStart, float phiLength)
    {
        this->points=points;
        this->segments=segments;
        this->phiStart=phiStart;
        this->phiLength=phiLength;

        LatheBufferGeometry::ptr bufferGeometry = std::make_shared<LatheBufferGeometry>(points, segments, phiStart, phiLength);
        this->fromBufferGeometry(*bufferGeometry);
        this->mergeVertices();
    }
}
