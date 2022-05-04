#include "ConvexGeometry.h"
#include <three/math/ConvexHull.h>
namespace three {
	ConvexBufferGeometry::ConvexBufferGeometry(std::vector<Vector3>& points) : BufferGeometry()
	{
		std::vector<float> _vertices;
		std::vector<float> _normals;
		ConvexHull convexHull;
		convexHull.setFromPoints(points);

        auto faces = convexHull.faces;

        for (int i = 0; i < faces.size(); i++)
        {
            auto face = faces[i];
            auto edge = face->edge;

            do
            {
                auto point = edge->head()->point;
                _vertices.push_back(point.x); _vertices.push_back(point.y); _vertices.push_back(point.z);
                _normals.push_back(face->normal.x); _normals.push_back(face->normal.y); _normals.push_back(face->normal.z);

                edge = edge->next;

            } while (edge != face->edge);
        }

        setAttribute(AttributeName::position,BufferAttribute<float>::create(_vertices, 3));
        setAttribute(AttributeName::normal,BufferAttribute<float>::create(_normals, 3));
        
	}
    ConvexGeometry::ConvexGeometry(std::vector<Vector3>& points) : Geometry()
    {
        ConvexBufferGeometry::ptr bufferGeometry = std::make_shared<ConvexBufferGeometry>(points);
        fromBufferGeometry(*bufferGeometry);
        mergeVertices();

    }
}
