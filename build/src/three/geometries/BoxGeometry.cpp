#include "BoxGeometry.h"
namespace three {
	void BoxBufferGeometry::buildPlane(unsigned char u, unsigned char v, unsigned char w, int udir, int vdir, float width, float height, float depth, int gridX, int gridY, int materialIndex)
	{
        auto segmentWidth = width / gridX;
        auto segmentHeight = height / gridY;

        auto widthHalf = width / 2;
        auto heightHalf = height / 2;
        auto depthHalf = depth / 2;

        auto gridX1 = gridX + 1;
        auto gridY1 = gridY + 1;

        auto vertexCounter = 0;
        auto groupCount = 0;

        int ix, iy;

        Vector3 vector;

        // generate vertices, normals and uvs

        for (iy = 0; iy < gridY1; iy++)
        {

            auto y = iy * segmentHeight - heightHalf;

            for (ix = 0; ix < gridX1; ix++)
            {

                auto x = ix * segmentWidth - widthHalf;


                // set values to correct vector component

                vector[u] = x * udir;
                vector[v] = y * vdir;
                vector[w] = depthHalf;

                // now apply vector to vertex buffer
                _vertices.push_back(vector.x);
                _vertices.push_back(vector.y);
                _vertices.push_back(vector.z);
               

                // set values to correct vector component

                vector[u] = 0;
                vector[v] = 0;
                vector[w] = depth > 0 ? 1 : -1;

                // now apply vector to normal buffer
                _normals.push_back(vector.x);
                _normals.push_back(vector.y);
                _normals.push_back(vector.z);



                // uvs
                _uvs.push_back(ix / gridX);               
                _uvs.push_back(1 - (iy / gridY));

                // counters

                vertexCounter += 1;

            }

        }


        // indices

        // 1. you need three indices to draw a single face
        // 2. a single segment consists of two faces
        // 3. so we need to generate six (2*3) indices per segment

        for (iy = 0; iy < gridY; iy++)
        {

            for (ix = 0; ix < gridX; ix++)
            {

                auto a = numberOfVertices + ix + gridX1 * iy;
                auto b = numberOfVertices + ix + gridX1 * (iy + 1);
                auto c = numberOfVertices + (ix + 1) + gridX1 * (iy + 1);
                auto d = numberOfVertices + (ix + 1) + gridX1 * iy;

                // faces

                _indices.push_back(a);
                _indices.push_back(b);
                _indices.push_back(d);

                _indices.push_back(b);
                _indices.push_back(c);
                _indices.push_back(d);

                // increase counter
                groupCount += 6;

            }

        }

        // add a group to the geometry. this will ensure multi material support

        addGroup(groupStart, groupCount, materialIndex);

        // calculate new start value for groups

        groupStart += groupCount;

        // update total number of vertices

        numberOfVertices += vertexCounter;
	}
    BoxBufferGeometry::BoxBufferGeometry(float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments) : BufferGeometry()
    {

        this->width= width;
        this->height= height;
        this->depth= depth;
        this->widthSegments= widthSegments;
        this->heightSegments= heightSegments;
        this->depthSegments= depthSegments;

        this->width = width ? width : 1;
        this->height = height ? height : 1;
        this->depth = depth ? depth : 1;

        widthSegments = floor(widthSegments) > 0 ? widthSegments : 1;
        heightSegments = floor(heightSegments) > 0 ? heightSegments : 1;
        depthSegments = floor(depthSegments) > 0 ? depthSegments : 1;

        buildPlane('z', 'y', 'x', -1, -1, depth, height, width, depthSegments, heightSegments, 0); // px
        buildPlane('z', 'y', 'x', 1, -1, depth, height, -width, depthSegments, heightSegments, 1); // nx
        buildPlane('x', 'z', 'y', 1, 1, width, depth, height, widthSegments, depthSegments, 2); // py
        buildPlane('x', 'z', 'y', 1, -1, width, depth, -height, widthSegments, depthSegments, 3); // ny
        buildPlane('x', 'y', 'z', 1, -1, width, height, depth, widthSegments, heightSegments, 4); // pz
        buildPlane('x', 'y', 'z', -1, -1, width, height, -depth, widthSegments, heightSegments, 5); // nz

        index = BufferAttribute<unsigned>::create(_indices, 1);
        setAttribute(AttributeName::position,BufferAttribute<float>::create(_vertices, 3));
        setAttribute(AttributeName::normal,BufferAttribute<float>::create(_normals, 3));
        setAttribute(AttributeName::uv,BufferAttribute<float>::create(_uvs, 2));


    }
    BoxGeometry::BoxGeometry(float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments) : Geometry()
    {

        this->width= width;
        this->height= height;
        this->depth= depth;
        this->widthSegments= widthSegments;
        this->heightSegments= heightSegments;
        this->depthSegments= depthSegments;

        BoxBufferGeometry::ptr bufferGeometry = BoxBufferGeometry::create(width, height, depth, widthSegments, heightSegments, depthSegments);

        this->fromBufferGeometry(*bufferGeometry);
        this->mergeVertices();
    }
}
