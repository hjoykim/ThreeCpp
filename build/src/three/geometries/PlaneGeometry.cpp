#include "PlaneGeometry.h"

three::PlaneBufferGeometry::PlaneBufferGeometry(float width, float height, float widthSegments, float heightSegments) : BufferGeometry()
{
    this->width= width;
    this->height= height;
    this->widthSegments= widthSegments;
    this->heightSegments= heightSegments;

    width = width == 0 ? 1 : width;
    height = height == 0 ? 1 : height;

    widthSegments = widthSegments == 0 ? 1 : widthSegments;
    heightSegments = heightSegments == 0 ? 1 : heightSegments;

    parameters.width = width;
    parameters.height = height;
    parameters.widthSegments = widthSegments;
    parameters.heightSegments = heightSegments;

    auto width_half = width / 2;
    auto height_half = height / 2;



    int gridX = (int)floor(widthSegments);
    int gridY = (int)floor(heightSegments);

    int gridX1 = gridX + 1;
    int gridY1 = gridY + 1;

    auto segment_width = width / gridX;
    auto segment_height = height / gridY;

    std::vector<unsigned> _indices;
    std::vector<float> _vertices;
    std::vector<float> _normals;
    std::vector<float> _uvs;

    //generate vertices, normals and uvs

    for (unsigned iy = 0; iy < gridY1; iy++)
    {
        auto y = iy * segment_height - height_half;

        for (unsigned ix = 0; ix < gridX1; ix++)
        {
            auto x = ix * segment_width - width_half;

            _vertices.push_back(x);
            _vertices.push_back(-y);
            _vertices.push_back(0);

            _normals.push_back(0);
            _normals.push_back(0);
            _normals.push_back(1);

            _uvs.push_back(ix / (float)gridX);
            _uvs.push_back(1 - (iy / (float)gridY));
        }
    }

    // _indices
    for (int iy = 0; iy < gridY; iy++)
    {
        for (int ix = 0; ix < gridX; ix++)
        {
            auto a = ix + gridX1 * iy;
            auto b = ix + gridX1 * (iy + 1);
            auto c = (ix + 1) + gridX1 * (iy + 1);
            auto d = (ix + 1) + gridX1 * iy;

            //faces
            _indices.push_back(a); _indices.push_back(b); _indices.push_back(d);
            _indices.push_back(b); _indices.push_back(c); _indices.push_back(d);
        }
    }

    setIndex(BufferAttribute<unsigned>::create(_indices, 1));
    setAttribute(AttributeName::position,BufferAttribute<float>::create(_vertices, 3));
    setAttribute(AttributeName::normal,BufferAttribute<float>::create(_normals, 3));
    setAttribute(AttributeName::uv,BufferAttribute<float>::create(_uvs, 2));
}

three::PlaneGeometry::PlaneGeometry(float width, float height, float widthSegments, float heightSegments) :Geometry()
{
    this->width= width;
    this->height= height;
    this->widthSegments= widthSegments;
    this->heightSegments= heightSegments;

    parameters.width = width;
    parameters.height = height;
    parameters.widthSegments = widthSegments;
    parameters.heightSegments = heightSegments;

    
    PlaneBufferGeometry::ptr bufferGeometry = PlaneBufferGeometry::create(width, height, widthSegments, heightSegments);

    this->fromBufferGeometry(*bufferGeometry);
    this->mergeVertices();

    bufferGeometry.reset();
}
