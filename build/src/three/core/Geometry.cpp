#include "Geometry.h"
#include <three/core/Object3D.h>
#include <three/objects/Mesh.h>
#include <three/core/BufferGeometry.h>
#include <three/core/DirectGeometry.h>
#include <functional>
namespace three {	
	namespace geometry {
		three::math::Matrix4 _m1;
		Object3D _obj;
		Vector3 _offset;
	}
	
	using namespace geometry;
	void Geometry::addFace(unsigned a, unsigned b, unsigned c, unsigned materialIndex, const std::shared_ptr<BufferAttribute<float>>& normals, const std::shared_ptr<BufferAttribute<float>>& uvs, const std::shared_ptr<BufferAttribute<float>>& uvs2)
	{
		std::vector<Color> vertexColors;
		if (this->colors.size() > 0)
		{
			vertexColors.push_back(colors[a]);
			vertexColors.push_back(colors[b]);
			vertexColors.push_back(colors[c]);
		}

		std::vector<Vector3>  vertexNormals;

		if (normals!=nullptr&&normals->size > 0)
		{
			vertexNormals.push_back(Vector3().fromArray(normals->array, a * 3));
			vertexNormals.push_back(Vector3().fromArray(normals->array, b * 3));
			vertexNormals.push_back(Vector3().fromArray(normals->array, c * 3));
		}

		auto face = Face3(a, b, c, &vertexNormals, &vertexColors, materialIndex);

		this->faces.push_back(face);

		if (uvs!=nullptr&&uvs->size > 0)
		{
			std::vector<Vector2> list2;
			list2.push_back(Vector2().fromArray(uvs->array, a * 2));
			list2.push_back(Vector2().fromArray(uvs->array, b * 2));
			list2.push_back(Vector2().fromArray(uvs->array, c * 2));
			std::vector<std::vector<Vector2>> list1;
			
			list1.push_back(list2);
			if (this->faceVertexUvs.size() == 0)
				this->faceVertexUvs.push_back(list1);
			else
				this->faceVertexUvs[0].push_back(list2);

		}
		if (uvs2!=nullptr&&uvs2->size > 0)
		{
			std::vector<Vector2> list2;
			list2.push_back(Vector2().fromArray(uvs2->array, a * 2));
			list2.push_back(Vector2().fromArray(uvs2->array, b * 2));
			list2.push_back(Vector2().fromArray(uvs2->array, c * 2));
			std::vector<std::vector<Vector2>> list1;
			list1.push_back(list2);
			if (this->faceVertexUvs.size() == 1)
				this->faceVertexUvs.push_back(list1);
			else
				this->faceVertexUvs[1].push_back(list2);

		}
	}
	
	Geometry::Geometry() : id(_geometryId++) {

	}
	Geometry::Geometry(const Geometry& source) :Geometry()
	{
		// reset		
		this->vertices.clear();
		this->colors.clear();
		this->faces.clear();
		for (unsigned int i = 0;i < faceVertexUvs.size();i++) {
			for (unsigned int j = 0;j < faceVertexUvs[i].size();j++) {
				faceVertexUvs[i][j].clear();
			}
			faceVertexUvs[i].clear();
		}
		this->faceVertexUvs.clear();
		this->morphTargets.clear();
		this->morphNormals.clear();
		this->skinWeights.clear();
		this->skinIndices.clear();
		this->lineDistances.clear();
		this->boundingBox.makeEmpty();
		this->boundingSphere.makeEmpty();

		// name

		this->name = source.name;

		// vertices

		this->vertices = source.vertices;

		

		// colors

		this->colors = source.colors;

		

		// faces

		this->faces = source.faces;

		this->faceVertexUvs = source.faceVertexUvs;

		//// face vertex uvs

		//for (unsigned int i = 0;i < source.faceVertexUvs.size(); i++) {

		//	auto faceVertexUvs = source.faceVertexUvs[i];

		//	/*if (this->faceVertexUvs[i] == = undefined) {

		//		this->faceVertexUvs[i] = [];

		//	}*/

		//	for (unsigned int j = 0;j < faceVertexUvs.size();j++) {

		//		auto uvs = faceVertexUvs[j];
		//		std::vector<Vector2> uvsCopy;

		//		for (unsigned int k = 0;k < uvs.size();k++) {

		//			auto uv = uvs[k];

		//			uvsCopy.push_back(uv.clone());

		//		}
		//		std::copy(uvsCopy.begin(), uvsCopy.end(), faceVertexUvs[i].end());
		//		//this->faceVertexUvs[i].push(uvsCopy);

		//	}

		//}

		// morph targets

		this->morphTargets = source.morphTargets;

		

		// morph normals

		this->morphNormals = source.morphNormals;

		

		// skin weights

		this->skinWeights = source.skinWeights;

		this->skinIndices = source.skinIndices;

		this->lineDistances = source.lineDistances;

		this->boundingBox.copy(source.boundingBox);

		this->boundingSphere.copy(source.boundingSphere);
		

		// update flags

		this->elementsNeedUpdate = source.elementsNeedUpdate;
		this->verticesNeedUpdate = source.verticesNeedUpdate;
		this->uvsNeedUpdate = source.uvsNeedUpdate;
		this->normalsNeedUpdate = source.normalsNeedUpdate;
		this->colorsNeedUpdate = source.colorsNeedUpdate;
		this->lineDistancesNeedUpdate = source.lineDistancesNeedUpdate;
		this->groupsNeedUpdate = source.groupsNeedUpdate;
	}
	Geometry& Geometry::applyMatrix4(Matrix4& matrix)
	{
		// TODO: 여기에 return 문을 삽입합니다.
		Matrix3 normalMatrix = Matrix3().getNormalMatrix(matrix);

		for (unsigned i = 0;i<vertices.size();i++) {

			//Vector3 vertex = vertices[i];
			vertices[i].applyMatrix4(matrix);

		}

		for (unsigned i = 0, il = faces.size(); i < il; i++) {

			//Face3 face = faces[i];
			faces[i].normal.applyMatrix3(normalMatrix).normalize();

			for (unsigned j = 0;faces[i].vertexNormals.size();j++) {

				faces[i].vertexNormals[j].applyMatrix3(normalMatrix).normalize();

			}

		}

		if (!boundingBox.isEmpty()) {

			computeBoundingBox();

		}

		if (!boundingSphere.isEmpty()) {

			computeBoundingSphere();

		}

		verticesNeedUpdate = true;
		normalsNeedUpdate = true;

		return *this;

	}
	Geometry& Geometry::rotateX(float angle)
	{
		// rotate geometry around world x-axis

		_m1.makeRotationX(angle);

		applyMatrix4(_m1);

		return *this;
	}
	Geometry& Geometry::rotateY(float angle)
	{
		// rotate geometry around world y-axis

		_m1.makeRotationY(angle);

		applyMatrix4(_m1);

		return *this;
	}
	Geometry& Geometry::rotateZ(float angle)
	{
		// rotate geometry around world z-axis

		_m1.makeRotationZ(angle);

		applyMatrix4(_m1);

		return *this;
	}
	Geometry& Geometry::translate(float x, float y, float z)
	{
		// translate geometry

		_m1.makeTranslation(x, y, z);

		applyMatrix4(_m1);

		return *this;
	}
	Geometry& Geometry::scale(float x, float y, float z)
	{
		// scale geometry

		_m1.makeScale(x, y, z);

		applyMatrix4(_m1);

		return *this;
	}
	Geometry& Geometry::lookAt(Vector3& vector)
	{
		_obj.lookAt(vector);

		_obj.updateMatrix();

		applyMatrix4(_obj.matrix);

		return *this;
	}
	Geometry& Geometry::fromBufferGeometry(BufferGeometry& geometry)
	{
		// TODO: 여기에 return 문을 삽입합니다.

		auto indices = geometry.index != nullptr ? geometry.index->array : std::vector<unsigned>();

		
		BufferAttribute<float>::ptr position = geometry.getAttribute(AttributeName::position);

		if (position==nullptr)
		{
			//Trace.TraceError("THREE.Core.Geometry.FromBufferGeometry():Position attribute required for conversion.");
			return *this;
		}

		BufferAttribute<float>::ptr positions = geometry.getAttribute(AttributeName::position);
		BufferAttribute<float>::ptr normals = geometry.getAttribute(AttributeName::normal); // as BufferAttribute<float>)["array"] as float[];
		BufferAttribute<float>::ptr colors = geometry.getAttribute(AttributeName::color);// as BufferAttribute<float>)["array"] as float[];
		BufferAttribute<float>::ptr uvs = geometry.getAttribute(AttributeName::uv);// as BufferAttribute<float>)["array"] as float[];
		BufferAttribute<float>::ptr uvs2 = geometry.getAttribute(AttributeName::uv);//as BufferAttribute<float>)["array"] as float[];

		if (uvs2 != nullptr)
		{
			std::vector<Vector2> uvsList2;
			std::vector<std::vector<Vector2>> uvsList1;
			uvsList1.push_back(uvsList2);

			this->faceVertexUvs.push_back(uvsList1);
		}
		this->vertices.clear(); this->vertices.shrink_to_fit();
		this->vertices.reserve(positions->size * 3);
		if (colors != nullptr) {
			this->colors.clear(); this->colors.shrink_to_fit();
			this->colors.reserve(colors->size * 3);
		}
		for (unsigned int i = 0; i < positions->size; i += 3)
		{
			this->vertices.push_back(Vector3().fromArray(positions->array, i));
			if (colors != nullptr && colors->size > 0) {

				this->colors.push_back(Color(ColorKeywords::white).fromArray(colors->array, i));
			}
		}

		//var groups = geometry.Groups;

		if (geometry.groups.size() > 0)
		{
			for (unsigned int i = 0; i < geometry.groups.size(); i++)
			{
				auto group = geometry.groups[i];

				auto start = group.start;
				auto count = group.count;

				for (unsigned int j = start, j1 = start + count; j < j1; j += 3)
				{
					if (indices.size() > 0 && geometry.index->size > 0)
					{
						addFace(indices[j], indices[j + 1], indices[j + 2], group.materialIndex, normals, uvs, uvs2);
					}
					else
					{
						addFace(j, j + 1, j + 2, group.materialIndex, normals, uvs, uvs2);
					}
				}
			}
		}
		else
		{
			this->faces.clear(); this->faces.shrink_to_fit();
			
			if (geometry.index!= nullptr && geometry.index->size > 0)
			{
				this->faces.reserve(geometry.index->size * 3);
				for (unsigned int i = 0; i < geometry.index->size; i += 3)
				{
					addFace(indices[i], indices[i + 1], indices[i + 2], 0, normals, uvs, uvs2);
				}
			}
			else
			{
				this->faces.reserve(position->size * 3);
				for (unsigned int i = 0; i < positions->size / 3; i += 3)
				{
					addFace(i, i + 1, i + 2, 0, normals, uvs, uvs2);
				}
			}
		}

		this->computeFaceNormals();

		if (!geometry.boundingBox.isEmpty())
		{
			this->boundingBox = geometry.boundingBox.clone(&this->boundingBox);
		}

		if (!geometry.boundingSphere.isEmpty())
		{
			this->boundingSphere = geometry.boundingSphere.clone(&this->boundingSphere);
		}
		return *this;

	}
	Geometry& Geometry::center()
	{
		computeBoundingBox();

		boundingBox.getCenter(&_offset).negate();

		translate(_offset.x, _offset.y, _offset.z);

		return *this;
	}
	Geometry& Geometry::normalize()
	{
		computeBoundingSphere();

		Vector3 center = boundingSphere.center;
		float radius = boundingSphere.radius;

		float s = radius ==  0 ? 1 : 1.0f / radius;

		Matrix4 matrix;
		matrix.set(
			s, 0, 0, -s * center.x,
			0, s, 0, -s * center.y,
			0, 0, s, -s * center.z,
			0, 0, 0, 1
			);

		applyMatrix4(matrix);

		return *this;
	}
	void Geometry::computeFaceNormals()
	{
		Vector3 cb, ab;

		for (unsigned f = 0;f<faces.size();f++) {

			Face3* face = &faces[f];

			Vector3 vA = vertices[face->a];
			Vector3 vB = vertices[face->b];
			Vector3 vC = vertices[face->c];

			cb.subVectors(vC, vB);
			ab.subVectors(vA, vB);
			cb.cross(ab);

			cb.normalize();

			face->normal.copy(cb);

		}
	}
	void Geometry::computeVertexNormals(bool areaWeighted)
	{
		if (areaWeighted == false) areaWeighted = true;

		//var v, vl, f, fl, face, vertices;

		std::vector<Vector3> vertices1 = std::vector<Vector3>(this->vertices.size());

		

		for (unsigned v = 0;v<this->vertices.size();v++) {

			vertices1[v]= Vector3();

		}

		if (areaWeighted) {

			// vertex normals weighted by triangle areas
			// http://www.iquilezles.org/www/articles/normals/normals.htm

			Vector3 vA, vB, vC;
			Vector3 cb,ab;

			for (unsigned f = 0;f< this->faces.size();f++) {

				Face3 face = faces[f];

				vA = this->vertices[face.a];
				vB = this->vertices[face.b];
				vC = this->vertices[face.c];

				cb.subVectors(vC, vB);
				ab.subVectors(vA, vB);
				cb.cross(ab);

				vertices1[face.a].add(cb);
				vertices1[face.b].add(cb);
				vertices1[face.c].add(cb);

			}

		}
		else {

			computeFaceNormals();

			for (unsigned f = 0;f< this->faces.size(); f++) {

				Face3 face = this->faces[f];

				vertices1[face.a].add(face.normal);
				vertices1[face.b].add(face.normal);
				vertices1[face.c].add(face.normal);

			}

		}

		for (unsigned v = 0; v< this->vertices.size(); v++) {

			vertices1[v].normalize();

		}

		for (unsigned f = 0; f< this->faces.size(); f++) {

			Face3* face = &faces[f];

			//std::vector<Vector3> vertexNormals = face.vertexNormals;

			if (face->vertexNormals.size() ==  3) {

				face->vertexNormals[0].copy(vertices1[face->a]);
				face->vertexNormals[1].copy(vertices1[face->b]);
				face->vertexNormals[2].copy(vertices1[face->c]);

			}
			else {				
				face->vertexNormals.clear();
				face->vertexNormals.push_back(vertices1[face->a].clone());
				face->vertexNormals.push_back(vertices1[face->b].clone());
				face->vertexNormals.push_back(vertices1[face->c].clone());

			}

		}

		if (this->faces.size() > 0) {

			this->normalsNeedUpdate = true;

		}
	}
	void Geometry::computeFlatVertexNormals()
	{
		computeFaceNormals();

		for (unsigned f = 0;f < faces.size(); f++)
		{

			Face3* face = &faces[f];

			//var vertexNormals = face.VertexNormals;

			if (face->vertexNormals.size() == 3)
			{

				face->vertexNormals[0].copy(face->normal);
				face->vertexNormals[1].copy(face->normal);
				face->vertexNormals[2].copy(face->normal);

			}
			else
			{
				face->vertexNormals.clear();
				face->vertexNormals.push_back(face->normal.clone());
				face->vertexNormals.push_back(face->normal.clone());
				face->vertexNormals.push_back(face->normal.clone());

			}

		}

		if (this->faces.size() > 0)
		{

			normalsNeedUpdate = true;

		}
	}
	void Geometry::computeMorphNormals()
	{
		//var i, il, f, fl, face;
		unsigned i, f;
		
		// save original normals
		// - create temp variables on first access
		//   otherwise just copy (for faster repeated calls)

		for (f = 0;f< this->faces.size();f++) {

			Face3* face = &faces[f];
			face->__originalFaceNormal.copy(face->normal);
						

			for (i = 0;i < face->vertexNormals.size();i++) {
				face->__originalVertexNormals[i].copy(face->vertexNormals[i]);
			}
		}

		// use temp geometry to compute face and vertex normals for each morph

		Geometry tmpGeo = Geometry();
		tmpGeo.faces = this->faces;

		for (i = 0;i< this->morphTargets.size();i++) {

			// create on first access

			//if (!this.morphNormals[i]) {

			/*this.morphNormals[i] = {};
			this.morphNormals[i].faceNormals = [];
			this.morphNormals[i].vertexNormals = [];*/

			auto dstNormalsFace = &morphNormals[i].faceNormals;
			auto dstNormalsVertex = &morphNormals[i].vertexNormals;

			//Vector3 faceNormal, vertexNormals;

			for (f = 0;f< this->faces.size(); f++) {

				Vector3 faceNormal = Vector3();
				std::vector<Vector3> vertexNormals = {Vector3(), Vector3(), Vector3() };

				dstNormalsFace->push_back(faceNormal);
				dstNormalsVertex->push_back(vertexNormals);

			}

			//}

			MorphNormals* morphNormals = &this->morphNormals[i];

			// set vertices to morph target

			tmpGeo.vertices = morphTargets[i].vertices;

			// compute morph normals

			tmpGeo.computeFaceNormals();
			tmpGeo.computeVertexNormals();

			std::copy(tmpGeo.faces.begin(), tmpGeo.faces.end(), faces.begin());

			// store morph normals

			//var faceNormal, vertexNormals;

			for (f = 0;f<this->faces.size(); f++) {

				Face3 face = this->faces[f];

				Vector3* faceNormal = &morphNormals->faceNormals[f];

				std::vector<Vector3> vertexNormals = morphNormals->vertexNormals[f];

				faceNormal->copy(face.normal);				
				
				morphNormals->vertexNormals[f][0].copy(face.vertexNormals[0]);
				morphNormals->vertexNormals[f][1].copy(face.vertexNormals[1]);
				morphNormals->vertexNormals[f][2].copy(face.vertexNormals[2]);

			}

		}

		// restore original normals

		for (f = 0;f<this->faces.size(); f++) {

			Face3* face = &this->faces[f];

			face->normal = face->__originalFaceNormal;
			face->vertexNormals = face->__originalVertexNormals;

		}
	}
	void Geometry::computeBoundingBox()
	{
		boundingBox.setFromPoints(&vertices[0],vertices.size());
	}
	void Geometry::computeBoundingSphere()
	{
		boundingSphere.setFromPoints(&vertices[0],vertices.size());
	}
	void Geometry::merge(Geometry& geometry, Matrix4* matrix, unsigned materialIndexOffset)
	{
		/*if (!geometry.isGeometry) {

			console.error('THREE.Geometry.merge(): geometry not an instance of THREE.Geometry.', geometry);
			return;

		}*/
		assert(!geometry.isGeometry);

		Matrix3* normalMatrix = nullptr;
		unsigned vertexOffset = this->vertices.size();
		auto vertices1 = &this->vertices;
		auto vertices2 = geometry.vertices;
		auto faces1 = &this->faces;
		auto faces2 = geometry.faces;
		auto colors1 = &this->colors;
		auto colors2 = geometry.colors;

		//if (materialIndexOffset == = undefined) materialIndexOffset = 0;

		if (matrix != nullptr) {

			normalMatrix = &Matrix3().getNormalMatrix(*matrix);

		}

		// vertices

		for (unsigned i = 0;i< vertices2.size(); i++) {

			auto vertex = vertices2[i];

			auto vertexCopy = vertex.clone();

			if (matrix != nullptr) vertexCopy.applyMatrix4(*matrix);

			vertices1->push_back(vertexCopy);

		}

		// colors

		for (unsigned i = 0;i< colors2.size(); i++) {

			colors1->push_back(colors2[i].clone());

		}

		// faces

		for (unsigned i = 0;i< faces2.size(); i++) {

			Face3 face = faces2[i];
			
			Vector3 normal;
			Color color;
			auto faceVertexNormals = face.vertexNormals;
			auto faceVertexColors = face.vertexColors;

			Face3 faceCopy = Face3(face.a + vertexOffset, face.b + vertexOffset, face.c + vertexOffset);
			faceCopy.normal.copy(face.normal);

			if (normalMatrix !=  nullptr) {

				faceCopy.normal.applyMatrix3(*normalMatrix).normalize();

			}

			for (unsigned j = 0;j< faceVertexNormals.size(); j++) {

				normal = faceVertexNormals[j].clone();

				if (normalMatrix != nullptr) {

					normal.applyMatrix3(*normalMatrix).normalize();

				}

				faceCopy.vertexNormals.push_back(normal);

			}

			faceCopy.color.copy(face.color);

			for (unsigned j = 0;j< faceVertexColors.size(); j++) {

				color = faceVertexColors[j];
				faceCopy.vertexColors.push_back(color.clone());

			}

			faceCopy.materialIndex = face.materialIndex + materialIndexOffset;

			faces1->push_back(faceCopy);

		}

		// uvs

		for (unsigned i = 0;i< geometry.faceVertexUvs.size();i++) {

			auto faceVertexUvs2 = geometry.faceVertexUvs[i]; //--> std::vector<std::array<Vector2,4>>
			//faceVertexUvs2.clear();
			//if (this.faceVertexUvs[i] == = undefined) this.faceVertexUvs[i] = [];

			for (unsigned j = 0;j< faceVertexUvs2.size();j++) {

				auto uvs2 = faceVertexUvs2[j];
				std::vector<Vector2> uvsCopy;

				for (unsigned k = 0;k< uvs2.size(); k++) {

					uvsCopy.push_back(uvs2[k].clone());

				}
				//std::copy(uvsCopy.begin(), uvsCopy.end(), faceVertexUvs[i].end());
				this->faceVertexUvs[i].push_back(uvsCopy);

			}

		}
	}
	void Geometry::mergeMesh(Mesh& mesh)
	{
		assert(!mesh.isMesh);

		if (mesh.matrixAutoUpdate) mesh.updateMatrix();

		merge(*mesh.geometry, &mesh.matrix);
	}

	/*
	* Checks for duplicate vertices with hashmap.
	* Duplicated vertices are removed
	* and faces' vertices are updated.
	*/
	int Geometry::mergeVertices()
	{
		std::unordered_map<std::string,size_t> verticesMap; // Hashmap for looking up vertices by position coordinates (and making sure they are unique)
		std::vector<Vector3> unique;
		std::vector<int> changes;

		//var v, key;
		unsigned precisionPoints = 4; // number of decimal points, e.g. 4 for epsilon of 0.0001
		float precision = (float)std::pow(10.0f, precisionPoints);
		//var i, il, face;
		//var indices, j, jl;

		for (unsigned i = 0;i< this->vertices.size(); i++) {

			Vector3 v = this->vertices[i];
			std::string key = std::to_string(round(v.x * precision)) + std::string("_") + std::to_string(round(v.y * precision)) + std::string("_") + std::to_string(round(v.z * precision));
			
			if (!verticesMap.count(key)) {

				verticesMap.insert({ key,i });
				unique.push_back(this->vertices[i]);
				changes.push_back(unique.size() - 1);

			}
			else {

				//console.log('Duplicate vertex found. ', i, ' could be using ', verticesMap[key]);
				changes.push_back(changes[verticesMap[key]]);

			}

		}


		// if faces are completely degenerate after merging vertices, we
		// have to remove them from the geometry.
		std::vector<int> faceIndicesToRemove;

		for (unsigned i = 0;i< this->faces.size(); i++) {

			Face3* face = &this->faces[i];

			face->a = changes[face->a];
			face->b = changes[face->b];
			face->c = changes[face->c];

			std::array<unsigned, 3> indices = { face->a, face->b, face->c };

			// if any duplicate vertices are found in a Face3
			// we have to remove the face as nothing can be saved
			for (int n = 0; n < 3; n++) {

				if (indices[n] == indices[(n + 1) % 3]) {

					faceIndicesToRemove.push_back(i);
					break;

				}

			}

		}

		if (faceIndicesToRemove.size() > 0) {
			for (int i = faceIndicesToRemove.size() - 1; i >= 0; i--) {
				
				int idx = faceIndicesToRemove[i];

				faces.erase(faces.begin() + idx);
				//this->faces.splice(idx, 1);

				for (unsigned j = 0; j < this->faceVertexUvs.size(); j++) {

					this->faceVertexUvs[j].erase(faceVertexUvs[j].begin() + idx);

				}

			}
		}
		// Use unique set of vertices

		int diff = this->vertices.size() - unique.size();
		//this->vertices = unique;
		this->vertices.clear();
		this->vertices.resize(unique.size());
		std::copy(unique.begin(), unique.end(), this->vertices.begin());

		return diff;
	}
	Geometry& Geometry::setFromPoints(Vector2* points, unsigned length)
	{
		this->vertices.clear();

		for (unsigned i = 0;i< length; i++) {

			Vector2 point = points[i];
			vertices.push_back(Vector3(point.x, point.y, 0));

		}

		return *this;
	}
	Geometry& Geometry::setFromPoints(Vector3* points, unsigned length)
	{
		this->vertices.clear();

		for (unsigned i = 0;i < length; i++) {

			Vector3 point = points[i];

			vertices.push_back(Vector3(point.x, point.y, point.z!=0?point.z:0));

		}

		return *this;
	}
	void Geometry::sortFacesByMaterialIndex()
	{
		size_t length = this->faces.size();

		// tag faces

		for (size_t i = 0; i < length; i++) {

			faces[i]._id = i;

		}

		// sort faces

		

		std::sort(faces.begin(),faces.end(),materialIndexSort);

		// sort uvs
		std::vector<std::vector<Vector2>> uvs1;
		std::vector<std::vector<Vector2>> uvs2;
		std::vector<std::vector<Vector2>> newUvs1;
		std::vector<std::vector<Vector2>> newUvs2;

		bool ifNewUvs1 = this->faceVertexUvs.size() > 0;
		bool ifNewUvs2 = this->faceVertexUvs.size() > 1;

		if(ifNewUvs1)
			uvs1 = faceVertexUvs[0]; // vector<vector<Vector2>>
		if(ifNewUvs2)
			uvs2 = faceVertexUvs[1]; //// vector<vector<Vector2>>

		if (ifNewUvs1&&uvs1.size() == length) newUvs1 = std::vector<std::vector<Vector2>>();// = [];
		if (ifNewUvs2&&uvs2.size() == length) newUvs2 = std::vector<std::vector<Vector2>>();// = [];

		for (size_t i = 0; i < length; i++) {

			size_t id = faces[i]._id;

			if (ifNewUvs1) newUvs1.push_back(uvs1[id]);
			if (ifNewUvs2) newUvs2.push_back(uvs2[id]);

		}

		if (ifNewUvs1) {
			faceVertexUvs[0].clear();
			faceVertexUvs[0].resize(newUvs1.size());
			std::copy(newUvs1.begin(), newUvs1.end(), faceVertexUvs[0].begin());
		}
		if (ifNewUvs2) {
			faceVertexUvs[1].clear();
			faceVertexUvs[1].resize(newUvs2.size());
			std::copy(newUvs2.begin(), newUvs2.end(), faceVertexUvs[1].begin());
			//this.faceVertexUvs[1] = newUvs2;
		}
	}
	Geometry* Geometry::clone()
	{
		return new Geometry(*this);
	}
	Geometry& Geometry::copy(const Geometry& source)
	{

		// reset

		this->vertices.clear();
		this->colors.clear();
		this->faces.clear();
		for (unsigned int i = 0;i < faceVertexUvs.size();i++) {
			for (unsigned int j = 0;j < faceVertexUvs[i].size();j++) {
				faceVertexUvs[i][j].clear();
			}
			faceVertexUvs[i].clear();
		}
		this->faceVertexUvs.clear();
		this->morphTargets.clear();
		this->morphNormals.clear();
		this->skinWeights.clear();
		this->skinIndices.clear();
		this->lineDistances.clear();
		this->boundingBox.makeEmpty();
		this->boundingSphere.makeEmpty();

		// name

		this->name = source.name;

		// vertices

		auto vertices = source.vertices;

		for (unsigned int i = 0;i < vertices.size(); i++) {

			this->vertices.push_back(vertices[i].clone());

		}

		// colors

		auto colors = source.colors;

		for (unsigned int i = 0;i< colors.size(); i++) {

			this->colors.push_back(colors[i].clone());

		}

		// faces

		auto faces = source.faces;

		for (unsigned int i = 0;i< faces.size();i++) {

			this->faces.push_back(faces[i]);

		}

		// face vertex uvs
		//this->faceVertexUvs = source.faceVertexUvs;

		if (this->faceVertexUvs.size() != source.faceVertexUvs.size()) {
			this->faceVertexUvs.clear();
			this->faceVertexUvs.resize(source.faceVertexUvs.size());
		}

		for (unsigned int i = 0;i< source.faceVertexUvs.size(); i++) {
			
			auto faceVertexUvs1 = source.faceVertexUvs[i];

			/*if (this->faceVertexUvs[i] == = undefined) {

				this->faceVertexUvs[i] = [];

			}*/
			if (this->faceVertexUvs[i].size() != faceVertexUvs1.size()) {
				this->faceVertexUvs[i].clear();
				this->faceVertexUvs[i].resize(faceVertexUvs1.size());
			}
			for (unsigned int j = 0;j< faceVertexUvs1.size();j++) {
				auto uvs = faceVertexUvs1[j];
				if (this->faceVertexUvs[i][j].size() != uvs.size()) {
					this->faceVertexUvs[i][j].clear();
					this->faceVertexUvs[i][j].resize(uvs.size());
				}
				std::vector<Vector2> uvsCopy;

				for (unsigned int k = 0;k< uvs.size();k++) {

					auto uv = uvs[k];

					uvsCopy.push_back(uv.clone());

				}				
				std::copy(uvsCopy.begin(), uvsCopy.end(),this->faceVertexUvs[i][j].begin());
				//this->faceVertexUvs[i].push(uvsCopy);

			}

		}

		// morph targets

		auto morphTargets = source.morphTargets;

		for (unsigned int i = 0;i< morphTargets.size(); i++) {

			MorphTarget morphTarget;
			morphTarget.name = morphTargets[i].name;

			// vertices

			if (morphTargets[i].vertices.size()> 0 ) {

				//morphTarget.vertices = [];

				for (unsigned int j = 0;j< morphTargets[i].vertices.size(); j++) {

					morphTarget.vertices.push_back(morphTargets[i].vertices[j].clone());

				}

			}

			// normals

			if (morphTargets[i].vertices.size() > 0) {

				//morphTarget.normals = [];

				for (unsigned int j = 0;j< morphTargets[i].vertices.size(); j++) {

					morphTarget.vertices.push_back(morphTargets[i].vertices[j].clone());

				}

			}

			this->morphTargets.push_back(morphTarget);

		}

		// morph normals

		auto morphNormals = source.morphNormals;

		for (unsigned int i = 0;i< morphNormals.size();  i++) {

			MorphNormals morphNormal;

			// vertex normals

			if (morphNormals[i].vertexNormals.size()>0) {

				//morphNormal.vertexNormals = [];

				for (unsigned int j = 0;j< morphNormals[i].vertexNormals.size(); j++) {

					auto srcVertexNormal = morphNormals[i].vertexNormals[j];
					std::vector<Vector3> destVertexNormal;

					destVertexNormal.push_back(srcVertexNormal[0].clone());
					destVertexNormal.push_back(srcVertexNormal[1].clone());
					destVertexNormal.push_back(srcVertexNormal[2].clone());

					morphNormal.vertexNormals.push_back(destVertexNormal);

				}

			}

			// face normals

			if (morphNormals[i].faceNormals.size()>0) {

				//morphNormal.faceNormals = [];

				for (unsigned int j = 0;j< morphNormals[i].faceNormals.size(); j++) {

					morphNormal.faceNormals.push_back(morphNormals[i].faceNormals[j].clone());

				}

			}

			this->morphNormals.push_back(morphNormal);

		}

		// skin weights

		//auto skinWeights = source.skinWeights;

		std::copy(source.skinWeights.begin(), source.skinWeights.end(), skinWeights.begin());
		/*for (int i = 0, il = skinWeights.length; i < il; i++) {

			this->skinWeights.push(skinWeights[i].clone());

		}*/

		// skin indices

		std::copy(source.skinIndices.begin(), source.skinIndices.end(), skinIndices.begin());
		/*var skinIndices = source.skinIndices;

		for (i = 0, il = skinIndices.length; i < il; i++) {

			this->skinIndices.push(skinIndices[i].clone());

		}*/

		// line distances

		std::copy(source.lineDistances.begin(), source.lineDistances.end(), lineDistances.begin());
		/*var lineDistances = source.lineDistances;

		for (i = 0, il = lineDistances.length; i < il; i++) {

			this->lineDistances.push(lineDistances[i]);

		}*/

		// bounding box

		/*var boundingBox = source.boundingBox;

		if (boundingBox != = null) {*/

			boundingBox.clone(&boundingBox);

		//}

		// bounding sphere

		//var boundingSphere = source.boundingSphere;

		//if (boundingSphere != = null) {

			boundingSphere.clone(&boundingSphere);

		//}

		// update flags

		this->elementsNeedUpdate = source.elementsNeedUpdate;
		this->verticesNeedUpdate = source.verticesNeedUpdate;
		this->uvsNeedUpdate = source.uvsNeedUpdate;
		this->normalsNeedUpdate = source.normalsNeedUpdate;
		this->colorsNeedUpdate = source.colorsNeedUpdate;
		this->lineDistancesNeedUpdate = source.lineDistancesNeedUpdate;
		this->groupsNeedUpdate = source.groupsNeedUpdate;

		return *this;
	}
}