#include "DirectGeometry.h"
#include <unordered_map>

namespace three {
	void three::DirectGeometry::computeGroups(const Geometry& geometry)
	{
        auto faces = geometry.faces;

        DrawRange group;
        group.start = -1;

        std::vector<DrawRange> groups;

        int materialIndex = -1;
        unsigned i;
        for (i = 0; i < faces.size(); i++)
        {
            auto face = faces[i];

            if (face.materialIndex != materialIndex)
            {
                materialIndex = face.materialIndex;

                if (group.start != -1)
                {

                    group.count = (i * 3) - group.start;
                    groups.push_back(group);
                }
                group = DrawRange(i * 3, 0 ,materialIndex );
            }
        }
        if (group.start != -1)
        {
            group.count = (i * 3) - group.start;
            groups.push_back(group);
        }
        if (groups.size() > 0) this->groups.resize(groups.size());

        std::copy(groups.begin(), groups.end(), this->groups.begin());
	}
	DirectGeometry& DirectGeometry::fromGeometry(Geometry& geometry)
	{
        auto faces = geometry.faces;
        auto vertices = geometry.vertices;
        auto faceVertexUvs = geometry.faceVertexUvs;

        bool hasFaceVertexUv = faceVertexUvs.size() > 0 && faceVertexUvs[0].size() > 0;
        bool hasFaceVertexUv2 = faceVertexUvs.size() > 1 && faceVertexUvs[1].size() > 0;


        auto morphTargets = geometry.morphTargets;

        auto morphTargetsLength = geometry.morphTargets.size();

        std::vector<MorphTarget> morphTargetsPosition;
        std::vector<std::string> morphTargetKeys;
        if (morphTargetsLength > 0)        {
            morphTargetsPosition.clear();
            for (unsigned i = 0; i < morphTargetsLength;i++) {
                
                MorphTarget target; target.name = morphTargets[i].name;
                morphTargetsPosition.push_back(target);
                morphTargetKeys.push_back(target.name);
            }
            
            this->morphPositionNormal.insert({ "position", morphTargetsPosition });
        }


        auto morphNormals = geometry.morphNormals;
        auto morphNormalsLength = morphNormals.size();

        std::vector<MorphTarget> morphTargetsNormal;
        std::vector<std::string> morphTargetsNormalKeys;
        //List<MorphTarget> morphTargetsNormal = new List<MorphTarget>();
        //List<string> morphTargetsNormalKeys = new List<string>();
        if (morphNormalsLength > 0)
        {
            for(unsigned i=0;i<morphNormalsLength;i++)
            {
                MorphTarget target; target.name = morphNormals[i].name;
                morphTargetsNormal.push_back(target);
                morphTargetsNormalKeys.push_back(target.name);
            }
            this->morphPositionNormal.insert({ "normal", morphTargetsNormal });
        }

        auto skinIndices = geometry.skinIndices;
        auto skinWeights = geometry.skinWeights;

        auto hasSkinIndices = skinIndices.size() == vertices.size();
        auto hasSkinWeights = skinWeights.size() == vertices.size();

       /* if (vertices.size() > 0 && faces.size() == 0)
        {
            Trace.TraceError("THREE.Core.DirectGeometry:Faceless geometries are not supported.");
        }*/
        assert(vertices.size() > 0 || faces.size() == 0);

        int vLen = vertices.size() - 1;
        for (unsigned i = 0; i < faces.size(); i++)
        {
            auto face = faces[i];
            if ((int)face.a > vLen) 
                continue;
            this->vertices.push_back(vertices[face.a]);
            this->vertices.push_back(vertices[face.b]);
            this->vertices.push_back(vertices[face.c]);

            auto vertexNormals = face.vertexNormals;

            if (vertexNormals.size() == 3)
            {
                this->normals.push_back(vertexNormals[0]);
                this->normals.push_back(vertexNormals[1]);
                this->normals.push_back(vertexNormals[2]);
            }
            else
            {
                auto normal = face.normal;

                this->normals.push_back(normal);
                this->normals.push_back(normal);
                this->normals.push_back(normal);
            }

            auto vertexColors = face.vertexColors;

            if (vertexColors.size() == 3)
            {
                this->colors.push_back(vertexColors[0]);
                this->colors.push_back(vertexColors[1]);
                this->colors.push_back(vertexColors[2]);
            }
            else
            {
                auto color = face.color;

                this->colors.push_back(color);
                this->colors.push_back(color);
                this->colors.push_back(color);
            }

            if (hasFaceVertexUv == true)
            {
                auto vertexUvs = faceVertexUvs[0][i];

                if (vertexUvs.size() > 0)
                {
                    this->uvs.push_back(vertexUvs[0]);
                    this->uvs.push_back(vertexUvs[1]);
                    this->uvs.push_back(vertexUvs[2]);
                }
                else
                {
                    //Trace.TraceError("THREE.Core.DirectGeometry.FromGeometry():undefined vertexUV");

                    this->uvs.push_back(Vector2());
                    this->uvs.push_back(Vector2());
                    this->uvs.push_back(Vector2());
                }
            }

            if (hasFaceVertexUv2 == true)
            {
                auto vertexUvs = faceVertexUvs[1][i];

                if (vertexUvs.size() > 0)
                {
                    this->uvs2.push_back(vertexUvs[0]);
                    this->uvs2.push_back(vertexUvs[1]);
                    this->uvs2.push_back(vertexUvs[2]);
                }
                else
                {
                    //Trace.TraceError("THREE.Core.DirectGeometry.FromGeometry():undefined vertexUV2");

                    this->uvs2.push_back(Vector2());
                    this->uvs2.push_back(Vector2());
                    this->uvs2.push_back(Vector2());
                }
            }

            // morphs
            for (unsigned j = 0; j < morphTargetKeys.size(); j++)
            {
                std::string key = morphTargetKeys[j];
                auto morphTarget = geometry.morphTargets[j].vertices;

                morphTargetsPosition[j].vertices.push_back(morphTarget[face.a]);
                morphTargetsPosition[j].vertices.push_back(morphTarget[face.b]);
                morphTargetsPosition[j].vertices.push_back(morphTarget[face.c]);
            }

            for (unsigned int j = 0; j < morphTargetsNormalKeys.size(); j++)
            {
                std::string key = morphTargetsNormalKeys[j];

                auto morphNormal = geometry.morphNormals[j].normals;

                morphTargetsNormal[j].normals.push_back(morphNormal[face.a]);
                morphTargetsNormal[j].normals.push_back(morphNormal[face.b]);
                morphTargetsNormal[j].normals.push_back(morphNormal[face.c]);
            }

            //skins
            if (hasSkinIndices)
            {
                this->skinIndices.push_back(skinIndices[face.a]);
                this->skinIndices.push_back(skinIndices[face.b]);
                this->skinIndices.push_back(skinIndices[face.c]);
            }
            if (hasSkinWeights)
            {
                this->skinWeights.push_back(skinWeights[face.a]);
                this->skinWeights.push_back(skinWeights[face.b]);
                this->skinWeights.push_back(skinWeights[face.c]);
            }
        }

        this->computeGroups(geometry);

        this->verticesNeedUpdate = geometry.verticesNeedUpdate;
        this->normalsNeedUpdate = geometry.normalsNeedUpdate;
        this->colorsNeedUpdate = geometry.colorsNeedUpdate;
        this->uvsNeedUpdate = geometry.uvsNeedUpdate;

        //if (geometry.boundingSphere != null)
        //{

        boundingSphere.copy(geometry.boundingSphere);
        //}

        //if (geometry.BoundingBox != null)
        //{
        boundingBox.copy(geometry.boundingBox);// = (Box3)geometry.BoundingBox.Clone();
        //}

        return *this;
		
	}
}
