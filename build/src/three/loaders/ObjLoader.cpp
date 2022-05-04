#include "ObjLoader.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <three/utils/StopWatch.h>
#include <three/core/BufferGeometry.h>
#include <three/Materials.h>
#include <three/objects/Mesh.h>
#include <three/objects/Line.h>
#include <three/objects/Points.h>
namespace three {
    namespace objloader {
        Object::ObjectMaterial::ptr emptyMaterial = nullptr;
    }
    using namespace objloader;
    Object::ObjectMaterial::ptr Object::ObjectMaterial::clone(int _index)
    {
        auto cloned = std::make_shared<Object::ObjectMaterial>();
        cloned->name = name;
        cloned->index = _index;
        cloned->smooth = smooth;

        cloned->objMtlLib = objMtlLib;

        return cloned;
    }
    Object::ObjectMaterial::ptr& Object::currentMaterial()
    {
        if (materials.size() > 0)
            return materials[materials.size() - 1];
        return emptyMaterial;

    }
    Object::ObjectMaterial::ptr Object::startMaterial(const string& name, vector<string>& libraries)
    {
        auto previous = finalize(false);

        if (previous!=nullptr && (previous->inherited || previous->groupCount <= 0))
        {
            materials.erase(materials.begin() + previous->index);
        }

        Object::ObjectMaterial::ptr material = make_shared<Object::ObjectMaterial>();

        material->index = materials.size();
        material->name = name;
        material->stringLib = libraries[libraries.size() - 1];
        material->smooth = previous!=nullptr ? previous->smooth : smooth;
        material->groupStart = previous!=nullptr ? previous->groupEnd : 0;
        material->groupEnd = -1;
        material->groupCount = -1;
        material->inherited = false;

        materials.push_back(material);

        return material;
    }
    Object::ObjectMaterial::ptr& Object::finalize(bool end)
    {
        ObjectMaterial::ptr& lastMultiMaterial = this->currentMaterial();
        if (lastMultiMaterial!=nullptr && lastMultiMaterial->groupEnd == -1) {
            lastMultiMaterial->groupEnd = geometry.vertices.size() / 3;
            lastMultiMaterial->groupCount = lastMultiMaterial->groupEnd - lastMultiMaterial->groupStart;
            lastMultiMaterial->inherited = false;
        }

        // Ignore objects tail materials if no face declarations followed them before a new o/g started.
        if (end && materials.size() > 1) {

            for (int mi = (int)materials.size() - 1; mi >= 0; mi--) {

                if (materials[mi]->groupCount <= 0) {

                    materials.erase(materials.begin() + mi);

                }
            }
        }

        // Guarantee at least one empty material, this makes the creation later more straight forward.
        if (end && materials.size() == 0) {

            Object::ObjectMaterial::ptr material = make_shared<Object::ObjectMaterial>();
            material->name = "";
            material->smooth = smooth;
            materials.push_back(material);
        }
       
        return lastMultiMaterial;
    }
    int ObjectState::parseVertexIndex(int value, size_t len)
    {
        int index = value;

        return (index >= 0 ? index - 1 : index + len / 3) * 3;
    }
    int ObjectState::parseNormalIndex(int value, size_t len)
    {
        int index = value;
        return (index >= 0 ? index - 1 : index + len / 3) * 3;
    }
    int ObjectState::parseUVIndex(int value, size_t len)
    {
        int index = value;

        return (index >= 0 ? index - 1 : index + len / 2) * 2;
    }

    void addVector3(vector<float>& target, float x, float y, float z) {
        target.push_back(x);
        target.push_back(y);
        target.push_back(z);
    }

    void addVector2(vector<float>& target, float x, float y) {
        target.push_back(x);
        target.push_back(y);
    }

    void ObjectState::addVertex(int a, int b, int c)
    {
        auto& src = vertices;
        auto& dst = object->geometry.vertices;
        /*if (src.size() != dst.size())
            dst.resize(src.size());


        std::copy(src.begin() + a, src.begin() + a + 3, dst.begin() + a);
        std::copy(src.begin() + b, src.begin() + b + 3, dst.begin() + b);
        std::copy(src.begin() + c, src.begin() + c + 3, dst.begin() + c);*/


        addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
        addVector3(dst, src[b + 0], src[b + 1], src[b + 2]);
        addVector3(dst, src[c + 0], src[c + 1], src[c + 2]);
      
    }
    void ObjectState::addColor(int a, int b, int c)
    {
        auto& src = colors;
        auto& dst = object->geometry.colors;              

       /* if (src.size() != dst.size())
            dst.resize(src.size());

        std::copy(src.begin() + a, src.begin() + a + 3, dst.end());
        std::copy(src.begin() + b, src.begin() + b + 3, dst.end());
        std::copy(src.begin() + c, src.begin() + c + 3, dst.end());*/
        
        addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
        addVector3(dst, src[b + 0], src[b + 1], src[b + 2]);
        addVector3(dst, src[c + 0], src[c + 1], src[c + 2]);
       
    }
    void ObjectState::addUV(int a, int b, int c)
    {
        auto& src = uvs;
        auto& dst = object->geometry.uvs;
             

       /* if (src.size() != dst.size())
            dst.resize(src.size());

        std::copy(src.begin() + a, src.begin() + a + 2, dst.end());
        std::copy(src.begin() + b, src.begin() + b + 2, dst.end());
        std::copy(src.begin() + c, src.begin() + c + 2, dst.end()); */

        addVector2(dst, src[a + 0], src[a + 1]);
        addVector2(dst, src[b + 0], src[b + 1]);
        addVector2(dst, src[c + 0], src[c + 1]);
      
    }
    void ObjectState::addNormal(int a, int b, int c)
    {
        auto& src = normals;
        auto& dst = object->geometry.normals;
               
       /* if (src.size() != dst.size())
            dst.resize(src.size());

        std::copy(src.begin() + a, src.begin() + a + 3, dst.end());
        std::copy(src.begin() + b, src.begin() + b + 3, dst.end());
        std::copy(src.begin() + c, src.begin() + c + 3, dst.end());*/

        addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
        addVector3(dst, src[b + 0], src[b + 1], src[b + 2]);
        addVector3(dst, src[c + 0], src[c + 1], src[c + 2]);
       
    }
    ObjectState::ObjectState()
    {
        object = make_shared<Object>();
    }
    void ObjectState::startObject(const string& name, bool fromDeclaration)
    {
        if (this->object!=nullptr && this->object->fromDeclaration == false)
        {
            this->object->name = name;
            this->object->fromDeclaration = fromDeclaration != false;

            return;
        }

        auto previousMaterial = object->currentMaterial();

        if (this->object!=nullptr)
        {
            this->object->finalize(true);
        }

        this->object = make_shared<Object>();
        this->object->name = name;
        this->object->fromDeclaration = fromDeclaration != false;
           

        if (previousMaterial!=nullptr && previousMaterial->name != "")
        {
            Object::ObjectMaterial::ptr declared = previousMaterial->clone(0);
            declared->inherited = true;
            object->materials.push_back(declared);
        }

        this->objects.push_back(this->object);
    }
    void ObjectState::finalize()
    {
        if (object!=nullptr)
        {
            object->finalize(true);
        }
    }
    void ObjectState::addVertexPoint(int a)
    {
        auto& src = vertices;
        auto& dst = object->geometry.vertices;

       /* if (src.size() != dst.size())
            dst.resize(src.size());

        std::copy(src.begin() + a, src.begin() + a + 2, dst.end());*/

        addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
       
    }
    void ObjectState::addVertexLine(int a)
    {
        auto& src = vertices;
        auto& dst = object->geometry.vertices;

        /*
        if (src.size() != dst.size())
            dst.resize(src.size());

        std::copy(src.begin() + a, src.begin() + a + 2, dst.end());
        */
        
        addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
    }
    void ObjectState::addUVLine(int a)
    {
        auto& src = uvs;
        auto& dst = object->geometry.uvs;

       /* if (src.size() != dst.size())
            dst.resize(src.size());

        std::copy(src.begin() + a, src.begin() + a + 1, dst.end());*/
        addVector2(dst, src[a + 0], src[a + 1]);
    }
    void ObjectState::addPointGeometry(const vector<string>& _vertices)
    {
        object->geometry.type = "Points";

        auto vLen = _vertices.size();

        for (auto vi = 0; vi < _vertices.size(); vi++)
        {
            addVertexPoint(parseVertexIndex(atoi(_vertices[vi].c_str()), vLen));
        }
    }

    void ObjectState::addPointGeometry(const vector<float>& _vertices)
    {
        object->geometry.type = "Points";

        auto vLen = _vertices.size();

        for (auto vi = 0; vi < _vertices.size(); vi++)
        {
            addVertexPoint(parseVertexIndex((int)_vertices[vi], vLen));
        }
    }

    void ObjectState::addLineGeometry(const vector<string>& _vertices, const vector<string>& _uvs)
    {
        object->geometry.type = "Line";

        auto vLen = _vertices.size();
        auto uvLen = _uvs.size();

        for (auto vi = 0; vi < _vertices.size(); vi++)
        {

            addVertexLine(parseVertexIndex(atoi(_vertices[vi].c_str()), vLen));

        }

        if (_uvs.size()> 0)
        {
            for (auto uvi = 0; uvi < _uvs.size(); uvi++)
            {

                addUVLine(parseUVIndex(atoi(_uvs[uvi].c_str()), uvLen));

            }
        }
    }
    void ObjectState::addLineGeometry(const vector<float>& _vertices, vector<float> _uvs)
    {
        object->geometry.type = "Line";

        auto vLen = _vertices.size();
        auto uvLen = _uvs.size();

        for (auto vi = 0; vi < _vertices.size(); vi++)
        {

            addVertexLine(parseVertexIndex((int)_vertices[vi], vLen));

        }

        if (_uvs.size() > 0)
        {
            for (auto uvi = 0; uvi < _uvs.size(); uvi++)
            {

                addUVLine(parseUVIndex((int)_uvs[uvi], uvLen));

            }
        }
    }
    void OBJLoader::parseState(ObjectState& state)
    {       
        state.startObject("", false);

    }
    float OBJLoader::parseFloat(string value)
    {
        return atof(value.c_str());
    }
    int OBJLoader::parseInt(string value)
    {
        return atoi(value.c_str());
    }
    void ObjectState::addFace(int a, int b, int c, int ua, int ub, int uc, int na, int nb, int nc)
    {
        auto vLen = vertices.size();

        auto ia = parseVertexIndex(a, vLen);
        auto ib = parseVertexIndex(b, vLen);
        auto ic = parseVertexIndex(c, vLen);

        addVertex(ia, ib, ic);

        if (colors.size() > 0)
        {
            addColor(ia, ib, ic);
        }

        if (ua != numeric_limits<int>::quiet_NaN())
        {

            auto uvLen = uvs.size();
            ia = parseUVIndex(ua, uvLen);
            ib = parseUVIndex(ub, uvLen);
            ic = parseUVIndex(uc, uvLen);

            addUV(ia, ib, ic);
        }

        if (na != numeric_limits<int>::quiet_NaN())
        {

            // Normals are many times the same. If so, skip function call and parseInt.
            auto nLen = normals.size();
            ia = parseNormalIndex(na, nLen);

            ib = na == nb ? ia : parseNormalIndex(nb, nLen);
            ic = na == nc ? ia : parseNormalIndex(nc, nLen);

            addNormal(ia, ib, ic);

        }
    }
    void ObjectState::addFace(int a, int b, int c, int d, int ua, int ub, int uc, int ud, int na, int nb, int nc, int nd)
    {
        auto vLen = vertices.size();

        auto ia = parseVertexIndex(a, vLen);
        auto ib = parseVertexIndex(b, vLen);
        auto ic = parseVertexIndex(c, vLen);

        if (d == numeric_limits<int>::quiet_NaN())
        {
            addVertex(ia, ib, ic);
        }
        else
        {
            auto id = parseVertexIndex(d, vLen);

            addVertex(ia, ib, id);
            addVertex(ib, ic, id);
        }

        if (colors.size() > 0)
        {

            addColor(ia, ib, ic);

        }

        if (ua != numeric_limits<int>::quiet_NaN())
        {

            auto uvLen = uvs.size();
            ia = parseUVIndex(ua, uvLen);
            ib = parseUVIndex(ub, uvLen);
            ic = parseUVIndex(uc, uvLen);

            if (d == numeric_limits<int>::quiet_NaN())
            {
                addUV(ia, ib, ic);
            }
            else
            {
                auto id = parseUVIndex(ud, uvLen);

                addUV(ia, ib, id);
                addUV(ib, ic, id);
            }


        }

        if (na != numeric_limits<int>::quiet_NaN())
        {

            // Normals are many times the same. If so, skip function call and parseInt.
            auto nLen = normals.size();
            ia = parseNormalIndex(na, nLen);

            ib = na == nb ? ia : parseNormalIndex(nb, nLen);
            ic = na == nc ? ia : parseNormalIndex(nc, nLen);

            if (d == numeric_limits<int>::quiet_NaN())
            {
                addNormal(ia, ib, ic);
            }
            else
            {
                auto id = parseNormalIndex(nd, nLen);

                addNormal(ia, ib, id);
                addNormal(ib, ic, id);
            }

        }    
    }

    void OBJLoader::setMaterials(const MTLLoader::MaterialCreator& materials)
    {
        this->materials = materials;
    }
    Group::ptr OBJLoader::parse(const string& path)
    {
        StopWatch sw;

        ObjectState state;

        state.vertices.reserve(1000 * 64);
        state.normals.reserve(1000 * 64);
        state.uvs.reserve(1000 * 64);


        parseState(state);

        filesystem::path absoluteFilePath = filesystem::absolute(path);

        string filePath = absoluteFilePath.parent_path().string();

        ifstream infile;

        infile.open(path);

        stringstream ss;
        ss << infile.rdbuf();
        string textAll = ss.str();

        infile.close();

        if (textAll.find(R"(\r\n)") > -1) {
            textAll = replace_all(textAll, R"(\r\n)", R"(\n)");
        }
        if (textAll.find(R"(\\\n)") > -1) {
            textAll = replace_all(textAll, R"(\\\n)", "");
        }

        vector<string> lines = split(textAll, '\n');

        char lineFirstChar;

        auto intNan = numeric_limits<int>::quiet_NaN();

        vector<vector<string>> faceVertices;
        faceVertices.reserve(10000 * 64);
        vector<string> lineVertices;
        lineVertices.reserve(10000 * 64);
        vector<string> lineUVs;
        lineUVs.reserve(10000 * 64);

        for (int i = 0; i < lines.size(); i++) {
            //string line = trim(lines[i]);
            string& line = lines[i];
            float x, y, z;

            if (line.size() == 0 || line.empty()) continue;

            lineFirstChar = line.c_str()[0];

            if (lineFirstChar == '#') continue;

            if (lineFirstChar == 'v') {
                //vector<string>& data = split(line, ' ');

                switch (line.c_str()[0]) {
                case 'v':
                    sscanf_s(line.c_str(), "v %f %f %f", &x, &y, &z);
                    state.vertices.push_back(x);
                    state.vertices.push_back(y);
                    state.vertices.push_back(z);
                    break;
                default:
                    switch (line.c_str()[1]) {
                    case 'n':
                        sscanf_s(line.c_str(), "vn %f %f %f", &x, &y, &z);
                        state.normals.push_back(x);
                        state.normals.push_back(y);
                        state.normals.push_back(z);
                        break;
                    case 't':
                        sscanf_s(line.c_str(), "vt %f %f", &x, &y);
                        state.uvs.push_back(x);
                        state.uvs.push_back(y);
                        break;
                    }
                }
            }
            else if (lineFirstChar == 'f') {                
                vector<string>& vertexData = split(line, ' ');
                vertexData.erase(vertexData.begin());

                faceVertices.clear();
                if (vertexData.size() == 1) faceVertices[0] = vertexData;
                else {
                    for (int j = 0; j < vertexData.size(); j++) {
                        auto& vertex = vertexData[j];
                        if (vertex.size() > 0) {
                            auto& vertexParts = split(vertex, '/');
                            faceVertices.push_back(vertexParts);
                        }
                    }
                }
                auto& v1 = faceVertices[0];

                for (int j = 1; j < faceVertices.size() - 1; j++)
                {
                    auto& v2 = faceVertices[j];
                    auto& v3 = faceVertices[j + 1];

                    state.addFace(
                        parseInt(v1[0]), parseInt(v2[0]), parseInt(v3[0]),
                        v1.size() > 1 ? parseInt(v1[1]) : numeric_limits<int>::quiet_NaN(), v2.size() > 1 ? parseInt(v2[1]) : numeric_limits<int>::quiet_NaN(), v3.size() > 1 ? parseInt(v3[1]) : numeric_limits<int>::quiet_NaN(),
                        v1.size() > 2 ? parseInt(v1[2]) : numeric_limits<int>::quiet_NaN(), v2.size() > 2 ? parseInt(v2[2]) : numeric_limits<int>::quiet_NaN(), v3.size() > 2 ? parseInt(v3[2]) : numeric_limits<int>::quiet_NaN()
                    );
                }
            }
            else if (lineFirstChar == 'l')
            {
                auto& lineParts = split(line, ' ');
                lineParts.erase(lineParts.begin());
                lineVertices.clear();
                lineUVs.clear();

                if (line.find("/") == -1)
                {
                    lineVertices = lineParts;
                }
                else
                {
                    for (int li = 0; li < lineParts.size(); li++)
                    {
                        auto& parts = split(lineParts[li], '/');

                        if (parts[0] != "") lineVertices.push_back(parts[0]);
                        if (parts[1] != "") lineUVs.push_back(parts[1]);
                    }
                }
                state.addLineGeometry(lineVertices, lineUVs);
            }
            else if (lineFirstChar == 'p')
            {
                auto& lineData = line.substr(1);
                auto& pointData = split(lineData, ' ');

                state.addPointGeometry(pointData);
            }

            else if (lineFirstChar == 's')
            {
                auto& result = split(line, ' ');
                if (result.size() > 1)
                {
                    toLowerCase(result[1]);
                    state.object->smooth = (result[1] != "0" && result[1] != "off");
                }

                else
                {
                    state.object->smooth = true;
                }
                auto& material = state.object->currentMaterial();
                if (material->index != -1) material->smooth = state.object->smooth;
                continue;
            }

            else if (lineFirstChar == 'o' || lineFirstChar == 'g')
            {
                auto name = line.substr(2);                               
                state.startObject(name, true);
                continue;
            }
            else if (lineFirstChar == 'u') // usemtl
            {
                string mtlName = line.substr(7);
                state.object->startMaterial(mtlName, state.materialLibraries);
                continue;
            }
            else if (lineFirstChar == 'm') // mtllib
            {
                string matFileName = line.substr(7);
                string directory = filePath;
                string matPath = directory + "/" + matFileName;
                //state.Object.StartMaterial(matPath, state.MaterialLibraries);
                state.materialLibraries.push_back(matPath);
                continue;
            }
            else {
                if (line == "\0")
                    continue;
                else {
                    throw std::runtime_error("THREE.OBJLoader: Unexpected line:" + line);
                }
            }
        }

        state.finalize();

        Group::ptr container = Group::create();

        container->materialLibraries = state.materialLibraries;

        if (container->materialLibraries.size() > 0)
        {
            MTLLoader mtlLoader;
            for (int i = 0; i < container->materialLibraries.size(); i++)
            {
                string mtlPath = container->materialLibraries[i];
                if (FileExists(mtlPath))
                    mtlLoader.load(mtlPath);
            }
            setMaterials(mtlLoader.multiMaterialCreator);
        }

        for (size_t i = 0; i < state.objects.size(); i++)
        {
            auto& object = state.objects[i];
            auto& geometry = object->geometry;
            auto& materials = object->materials;
            bool isLine = geometry.type == "Line" ? true : false;
            bool isPoints = geometry.type == "Points" ? true : false;
            bool hasVertexColors = false;

            if (geometry.vertices.size() == 0) continue;

            auto bufferGeometry = BufferGeometry::create();

            bufferGeometry->setAttribute(AttributeName::position, BufferAttribute<float>::create(geometry.vertices, 3));

            if (geometry.normals.size() > 0)
            {
                bufferGeometry->setAttribute(AttributeName::normal, BufferAttribute<float>::create(geometry.normals, 3));
            }
            else
            {
                bufferGeometry->computeVertexNormals();
            }

            if (geometry.colors.size() > 0)
            {
                hasVertexColors = true;
                bufferGeometry->setAttribute(AttributeName::color, BufferAttribute<float>::create(geometry.colors, 3));
            }

            if (geometry.uvs.size() > 0)
            {
                bufferGeometry->setAttribute(AttributeName::uv, BufferAttribute<float>::create(geometry.uvs, 2));
            }

            // Create materials

            vector<Material::ptr> createdMaterials;

            for (int mi = 0; mi < materials.size(); mi++)
            {

                auto sourceMaterial = materials[mi];
                Material::ptr material = nullptr;

                if (materials.size() > 0 && !sourceMaterial->name.empty())
                {

                    material = this->materials.create(sourceMaterial->name);

                    // mtl etc. loaders probably can't create line materials correctly, copy properties to a line material.
                    if (isLine && material != nullptr && !instanceOf<LineBasicMaterial>(material.get()))
                    {

                        LineBasicMaterial::ptr materialLine = LineBasicMaterial::create();
                        materialLine->copy(*material);
                        material = materialLine;

                    }
                    else if (isPoints && material != nullptr && !instanceOf<PointsMaterial>(material.get()))
                    {

                        PointsMaterial::ptr materialPoints = PointsMaterial::create();
                        materialPoints->copy(*std::dynamic_pointer_cast<PointsMaterial>(material));
                        materialPoints->size = 10;
                        materialPoints->sizeAttenuation = false;
                        materialPoints->map = material->map;
                        material = materialPoints;
                    }

                }
                if (material == nullptr)
                {
                    if (isLine)
                    {
                        material = LineBasicMaterial::create();
                    }
                    else if (isPoints)
                    {
                        material = PointsMaterial::create();
                        material->size = 1;
                        material->sizeAttenuation = false;
                    }
                    else
                    {
                        material = MeshPhongMaterial::create();
                    }

                    material->name = sourceMaterial->name;
                }

                material->flatShading = sourceMaterial->smooth ? false : true;
                material->vertexColors = hasVertexColors ? true : false;

                createdMaterials.push_back(material);
            }

            // Create Mesh
            Mesh::ptr mesh;

            if (createdMaterials.size() > 1)
            {

                for (int mi = 0; mi < materials.size(); mi++)
                {

                    auto sourceMaterial = materials[mi];
                    bufferGeometry->addGroup(sourceMaterial->groupStart, sourceMaterial->groupCount, mi);

                }

                if (isLine)
                {

                    auto lineSegment = LineSegments::create(bufferGeometry, createdMaterials);
                    lineSegment->name = object->name;
                    container->add(lineSegment);
                }
                else if (isPoints)
                {

                    auto points = Points::create(bufferGeometry, createdMaterials);
                    points->name = object->name;
                    container->add(points);

                }
                else
                {

                    mesh = Mesh::create(bufferGeometry, createdMaterials);
                    mesh->name = object->name;
                    container->add(mesh);

                }

            }
            else
            {

                if (isLine)
                {

                    auto lineSegment = LineSegments::create(bufferGeometry, createdMaterials[0]);
                    lineSegment->name = object->name;
                    container->add(lineSegment);
                }
                else if (isPoints)
                {

                    auto points = Points::create(bufferGeometry, createdMaterials[0]);
                    points->name = object->name;
                    container->add(points);
                }
                else
                {

                    mesh = Mesh::create(bufferGeometry, createdMaterials[0]);
                    mesh->name = object->name;
                    container->add(mesh);
                }
            }
        }

        return container;
       
    }
    Group::ptr OBJLoader::load(const string& path)
    {
        return parse(path);
    }
}
