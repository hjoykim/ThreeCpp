#include "CylinderGeometry.h"

void three::CylinderBufferGeometry::generateTorso()
{
    int x, y;
    int groupCount = 0;

    Vector3 normal;
    Vector3 vertex;

    float slope = (this->radiusBottom - this->radiusTop) / this->height;
   /* this->_indexArray.resize(_indexArray.size()+ceil(this->radialSegments * this->heightSegments));
    this->_indices.resize(_indices.size() + ceil(this->radialSegments * this->heightSegments) * 6);
    this->_vertices.resize(_vertices.size() + ceil(this->radialSegments * this->heightSegments)*3);
    this->_normals.resize(_normals.size() + ceil(this->radialSegments * this->heightSegments) * 3);
    this->_uvs.resize(_uvs.size() + ceil(this->radialSegments * this->heightSegments) * 2);*/
    for (y = 0; y <= this->heightSegments; y++)
    {
        std::vector<int> indexRow;                

        float v = (float)y / (float)this->heightSegments;

        float radius = v * (this->radiusBottom - this->radiusTop) + this->radiusTop;

        for (x = 0; x <= this->radialSegments; x++)
        {
            float u = (float)x / (float)this->radialSegments;
            float theta = u * this->thetaLength + this->thetaStart;

            float sintheta = (float)sin(theta);
            float costheta = (float)cos(theta);

            //vertex

            vertex.x = radius * sintheta;
            vertex.y = -v * this->height + this->halfHeight;
            vertex.z = radius * costheta;
            _vertices.push_back(vertex.x); _vertices.push_back(vertex.y); _vertices.push_back(vertex.z);

            //normal

            normal.set(sintheta, slope, costheta).normalize();
            _normals.push_back(normal.x); _normals.push_back(normal.y); _normals.push_back(normal.z);

            //uv

            _uvs.push_back(u); _uvs.push_back(1 - v);
            indexRow.push_back(index++);
        }
        _indexArray.push_back(indexRow);
    }

    // generate indices

    for (x = 0; x < this->radialSegments; x++)
    {
        for (y = 0; y < this->heightSegments; y++)
        {
            int a = _indexArray[y][x];
            int b = _indexArray[y + 1][x];
            int c = _indexArray[y + 1][x + 1];
            int d = _indexArray[y][x + 1];

            _indices.push_back(a); _indices.push_back(b); _indices.push_back(d);
            _indices.push_back(b); _indices.push_back(c); _indices.push_back(d);

            groupCount += 6;
        }
    }

   /* _vertices.shrink_to_fit();
    _normals.shrink_to_fit();
    _uvs.shrink_to_fit();
    _indices.shrink_to_fit();*/

    this->addGroup(groupStart, groupCount, 0);

    groupStart += groupCount;
}

void three::CylinderBufferGeometry::generateCap(bool top)
{
    int x, centerIndexStart, centerIndexEnd;

    Vector2 uv;
    Vector3 vertex;

    int groupCount = 0;

    float radius = top ? this->radiusTop : this->radiusBottom;
    float sign = top ? 1 : -1;

    centerIndexStart = index;

    /*this->_indices.resize(_indices.size() + ceil(this->radialSegments) * 6);
    this->_vertices.resize(_vertices.size() + ceil(this->radialSegments) * 3);
    this->_normals.resize(_normals.size() + ceil(this->radialSegments) * 3);
    this->_uvs.resize(_uvs.size() + ceil(this->radialSegments) * 2);*/

    for (x = 1; x <= this->radialSegments; x++)
    {
        _vertices.push_back(0); _vertices.push_back(this->halfHeight * sign); _vertices.push_back(0);
        _normals.push_back(0); _normals.push_back(sign); _normals.push_back(0);
        _uvs.push_back(0.5f); _uvs.push_back(0.5f);
        index++;
    }

    centerIndexEnd = index;

    for (x = 0; x <= this->radialSegments; x++)
    {
        float u = (float)x / (float)this->radialSegments;
        float theta = u * this->thetaLength + this->thetaStart;

        float costheta = (float)cos(theta);
        float sintheta = (float)sin(theta);

        //vertex

        vertex.x = radius * sintheta;
        vertex.y = this->halfHeight * sign;
        vertex.z = radius * costheta;

        _vertices.push_back(vertex.x); _vertices.push_back(vertex.y); _vertices.push_back(vertex.z);

        //normal
        _normals.push_back(0); _normals.push_back(sign); _normals.push_back(0);

        //uv

        uv.x = (costheta * 0.5f) + 0.5f;
        uv.y = (sintheta * 0.5f * sign) + 0.5f;
        _uvs.push_back(uv.x); _uvs.push_back(uv.y);

        index++;
    }

    //generate indices

    for (x = 0; x < this->radialSegments; x++)
    {
        int c = centerIndexStart + x;
        int i = centerIndexEnd + x;

        if (top)
        {
            _indices.push_back(i); _indices.push_back(i + 1); _indices.push_back(c);
        }
        else
        {
            _indices.push_back(i + 1); _indices.push_back(i); _indices.push_back(c);
        }
        groupCount += 3;
    }

   /* _vertices.shrink_to_fit();
    _normals.shrink_to_fit();
    _uvs.shrink_to_fit();
    _indices.shrink_to_fit();*/

    this->addGroup(groupStart, groupCount, top ? 1 : 2);

    groupStart += groupCount;
}


three::CylinderBufferGeometry::CylinderBufferGeometry(float radiusTop, float radiusBottom, float height, float radialSegments, float heightSegments, bool openEnded, float thetaStart, float thetaLength) : BufferGeometry()
{
    this->radiusTop = radiusTop;
    this->radiusBottom = radiusBottom;
    this->height = height != 0 ? height : 1;
    this->radialSegments = !std::isnan(radialSegments) ? floor(radialSegments) : 8;
    this->heightSegments = !std::isnan(heightSegments) ? floor(heightSegments) : 1;

    this->openEnded = openEnded;

    this->thetaStart = !std::isnan(thetaStart) ? thetaStart : 0.0f;
    this->thetaLength = !std::isnan(thetaLength) ? thetaLength :(float)(M_PI * 2);

    this->halfHeight = this->height / 2;

    parameters.radiusTop = radiusTop;
    parameters.radiusBottom = radiusBottom;
    parameters.height = height;
    parameters.radialSegments = radialSegments;
    parameters.heightSegments = heightSegments;
    parameters.openEnded = openEnded;
    parameters.thetaStart = thetaStart;
    parameters.thetaLength = thetaLength;

    generateTorso();

    if (!this->openEnded) {
        if (this->radiusTop > 0) generateCap(true);
        if (this->radiusBottom > 0) generateCap(false);
    }

    setIndex(BufferAttribute<unsigned>::create(_indices, 1));

    setAttribute(AttributeName::position,BufferAttribute<float>::create(_vertices, 3));
    setAttribute(AttributeName::normal,BufferAttribute<float>::create(_normals, 3));
    setAttribute(AttributeName::uv,BufferAttribute<float>::create(_uvs, 2));

}

three::CylinderGeometry::CylinderGeometry(float radiusTop, float radiusBottom, float height, float radialSegments, float heightSegments, bool openEnded, float thetaStart, float thetaLength) : Geometry()
{
    this->radiusTop= radiusTop;
    this->radiusBottom= radiusBottom;
    this->height= height;
    this->radialSegments= radialSegments;
    this->heightSegments= heightSegments;
    this->openEnded= openEnded;
    this->thetaStart= thetaStart;
    this->thetaLength= thetaLength;

    parameters.radiusTop = radiusTop;
    parameters.radiusBottom = radiusBottom;
    parameters.height = height;
    parameters.radialSegments = radialSegments;
    parameters.heightSegments = heightSegments;
    parameters.openEnded = openEnded;
    parameters.thetaStart = thetaStart;
    parameters.thetaLength = thetaLength;

    CylinderBufferGeometry::ptr bufferGeometry = std::make_shared<CylinderBufferGeometry>(radiusTop,
        radiusBottom,
        height,
        radialSegments,
        heightSegments,
        openEnded,
        thetaStart,
        thetaLength);
    fromBufferGeometry(*bufferGeometry);
    mergeVertices();
}
