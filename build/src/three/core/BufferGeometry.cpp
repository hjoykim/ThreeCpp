#include "BufferGeometry.h"
#include <three/math/Box3.h>
#include <three/math/Vector3.h>
#include <three/core/Object3D.h>
#include <three/objects/Line.h>
#include <three/objects/Points.h>
#include <three/core/DirectGeometry.h>
#include <three/objects/Mesh.h>
using namespace three::math;

namespace three {
	namespace buffergeometry {
		Matrix4 _m1;
		Box3 _box;
		Box3 _boxMorphTargets;
		Vector3 _vector;
		Vector3 _offset;
		Object3D _obj;
		BufferAttribute<float>::ptr emptyAttribute;
	}
	using namespace buffergeometry;
	BufferGeometry::BufferGeometry() :Geometry()
	{
		type = "BufferGeometry";
		drawRange.start = 0;
		drawRange.materialIndex = -1;
		drawRange.count = std::numeric_limits<unsigned>::max();

		isGeometry = false;
	}
	BufferGeometry::BufferGeometry(const BufferGeometry& source) : Geometry(source) 	
	{
		
		type = "BufferGeometry";
		drawRange.start = 0;
		drawRange.materialIndex = -1;
		drawRange.count = std::numeric_limits<unsigned>::max();

		isGeometry = false;

		name = source.name;

		if(source.index) index.reset(source.index->clone());			

		for (auto attr = source.attributes.begin();attr != source.attributes.end();) {
			BufferAttribute<float>::ptr newAttr = std::make_shared<BufferAttribute<float>>();
			newAttr.reset(attr->second->clone());
			attributes[attr->first] = newAttr;
			attr++;
			
		}
		/*if(source.position) position.reset(source.position->clone());

		if(source.normal) normal.reset(source.normal->clone());

		if(source.color) color.reset(source.color->clone());

		if(source.uv) uv.reset(source.uv->clone());

		if(source.uv2) uv2.reset(source.uv2->clone());

		if(source.tangent) tangent.reset(source.tangent->clone());

		if(source.bitangent) bitangent.reset(source.bitangent->clone());

		if(source.lineDistance) lineDistance.reset(source.lineDistance->clone());*/
		drawCalls.assign(source.drawCalls.begin(), source.drawCalls.end());
		offsets.assign(source.offsets.begin(), source.offsets.end());
			

		
		for (auto& attr : source.morphAttributes) {
			std::vector<BufferAttribute<float>::ptr> array;
			for (auto ptr : attr.second) {
				auto positionPtr = BufferAttribute<float>::create();
				positionPtr->copy(*ptr);
				array.push_back(positionPtr);
			}
			morphAttributes.insert({ attr.first,array });
		}

		

		/*if(source.skinIndex) skinIndex.reset(source.skinIndex->clone());

		if(source.skinWeight) skinWeight.reset(source.skinWeight->clone());*/

		morphTargetsRelative = source.morphTargetsRelative;

		drawRange = source.drawRange;

		groups.assign(source.groups.begin(),source.groups.end());

	}

	BufferGeometry::BufferGeometry(const BufferAttribute<float>::ptr& position, const BufferAttribute<float>::ptr& color) : BufferGeometry()
	{

		this->setAttribute(AttributeName::position,position);
		this->setAttribute(AttributeName::color,color);
		
	}
	const BufferAttribute<unsigned>::ptr& BufferGeometry::getIndex()
	{
		return index;
	}
	BufferGeometry& BufferGeometry::setIndex(const BufferAttribute<unsigned>::ptr& indices)
	{
		this->index = indices;

		return *this;
	}
	/*BufferGeometry& BufferGeometry::setPosition(const BufferAttribute<float>::ptr& position)
	{
		this->position.reset();
		this->position = position;
		return *this;
	}
	BufferGeometry& BufferGeometry::setNormal(const BufferAttribute<float>::ptr& normal)
	{
		this->normal.reset();
		this->normal = normal;
		return *this;
	}
	BufferGeometry& BufferGeometry::setColor(const BufferAttribute<float>::ptr& color)
	{
		this->color.reset();
		this->color = color;
		return *this;
	}
	BufferGeometry& BufferGeometry::setUV(const BufferAttribute<float>::ptr& uv)
	{
		this->uv.reset();
		this->uv = uv;
		return *this;
	}
	BufferGeometry& BufferGeometry::setUV2(const BufferAttribute<float>::ptr& uv2)
	{
		this->uv2.reset();
		this->uv2 = uv2;
		return *this;
	}
	BufferGeometry& BufferGeometry::setTangent(const BufferAttribute<float>::ptr& tangent)
	{
		this->tangent.reset();
		this->tangent = tangent;
		return *this;
	}
	BufferGeometry& BufferGeometry::setBitangent(const BufferAttribute<float>::ptr& bitangent)
	{
		this->bitangent.reset();
		this->bitangent = bitangent;
		return *this;
	}
	BufferGeometry& BufferGeometry::setLineDistance(const BufferAttribute<float>::ptr& lineDistance)
	{
		this->lineDistance.reset();
		this->lineDistance = lineDistance;
		return *this;
	}
	BufferGeometry& BufferGeometry::setSkinIndexAttribute(const BufferAttribute<float>::ptr& skinIndexAttribute)
	{
		this->skinIndex.reset();
		this->skinIndex = skinIndexAttribute;

		return *this;
	}
	BufferGeometry& BufferGeometry::setSkinWeightAttribute(const BufferAttribute<float>::ptr skinWeightAttribute)
	{
		this->skinWeight.reset();
		this->skinWeight = skinWeightAttribute;
		return *this;
	}*/

	BufferAttribute<float>::ptr& BufferGeometry::getAttribute(AttributeName name)
	{
		return attributes[{name, 0}];
		/*switch (name) {
		case AttributeName::normal:
			return normal;
		case AttributeName::lineDistance:
			return lineDistance;		
		case AttributeName::uv:
			return uv;
		case AttributeName::uv2:
			return uv2;
		case AttributeName::color:
			return color;
		case AttributeName::position:
			return position;
		case AttributeName::tangent:
			return tangent;
		case AttributeName::bitangent :
			return bitangent;
		case AttributeName::skinIndex:
			return skinIndex;
		case AttributeName::skinWeight :
			return skinWeight;
		
		default:
			return nullptr;
		}*/
	}
	BufferAttribute<float>::ptr& BufferGeometry::getAttribute(const std::string& name)
	{
		if (name == "position")
			return getAttribute(AttributeName::position);
		else if (name == "normal")
			return getAttribute(AttributeName::normal);
		else if (name == "uv")
			return getAttribute(AttributeName::uv);
		else if (name == "uv2")
			return getAttribute(AttributeName::uv2);
		else if (name == "color")
			return getAttribute(AttributeName::color);
		else if (name == "tangent")
			return getAttribute(AttributeName::tangent);
		else if (name == "bitangent")
			return getAttribute(AttributeName::bitangent);
		else if (name == "lineDistance")
			return getAttribute(AttributeName::lineDistance);
		else if (name == "skinIndex")
			return getAttribute(AttributeName::skinIndex);
		else if (name == "skinWeigh")
			return getAttribute(AttributeName::skinWeight);

		
		return emptyAttribute;
	}

		
	void BufferGeometry::deleteAttribute(AttributeName name)
	{
		attributes.erase({ name,0 });
		/*switch (name) {
		case AttributeName::normal:
			normal.reset();
			normal = nullptr;
			break;
		case AttributeName::lineDistance:
			lineDistance.reset();
			lineDistance = nullptr;
			break;
		case AttributeName::uv:
			uv.reset();
			uv = nullptr;
			break;
		case AttributeName::uv2:
			uv2.reset();
			uv2 = nullptr;
			break;
		case AttributeName::color:
			color.reset();
			color = nullptr;
			break;
		case AttributeName::position:
			position.reset();
			position = nullptr;
			break;
		case AttributeName::tangent:
			tangent.reset();
			tangent = nullptr;
			break;
		case AttributeName::bitangent:
			bitangent.reset();
			bitangent = nullptr;
			break;
		case AttributeName::skinIndex:
			skinIndex.reset();
			skinIndex = nullptr;
			break;
		case AttributeName::skinWeight:
			skinWeight.reset();
			skinWeight = nullptr;
			break;

		default:
			return;
		}*/
	}

	void BufferGeometry::addGroup(const unsigned start, const unsigned count, const int materialIndex)
	{
		groups.push_back(DrawRange(start,count,materialIndex));
	}
	void BufferGeometry::clearGroups()
	{
		groups.clear();
	}
	void BufferGeometry::setDrawRange(const unsigned start, const unsigned count)
	{
		this->drawRange.start = start;
		this->drawRange.count = count;
	}
	BufferGeometry& BufferGeometry::applyMatrix4(const Matrix4& matrix)
	{

		BufferAttribute<float>::ptr position = getAttribute(AttributeName::position);

		if (getAttribute(AttributeName::position) != nullptr) {

			position->applyMatrix4(matrix);

			position->setNeedsUpdate(true);
		}

		//var normal = this.attributes.normal;
		auto normal = getAttribute(AttributeName::normal);
		if (normal != nullptr) {

			auto normalMatrix = Matrix3().getNormalMatrix(matrix);

			normal->applyNormalMatrix(normalMatrix);

			normal->setNeedsUpdate(true);

		}

		auto tangent = getAttribute(AttributeName::tangent);

		if (tangent != nullptr) {

			tangent->transformDirection(matrix);

			tangent->setNeedsUpdate(true);

		}

		if (!boundingBox.isEmpty()) {

			computeBoundingBox();

		}

		if (!boundingSphere.isEmpty()) {

			computeBoundingSphere();

		}

		return *this;
	}
	BufferGeometry& BufferGeometry::rotateX(const float angle)
	{
		_m1.makeRotationX(angle);

		this->applyMatrix4(_m1);

		return *this;
	}
	BufferGeometry& BufferGeometry::rotateY(const float angle)
	{
		_m1.makeRotationY(angle);

		this->applyMatrix4(_m1);

		return *this;
	}
	BufferGeometry& BufferGeometry::rotateZ(const float angle)
	{
		_m1.makeRotationZ(angle);

		this->applyMatrix4(_m1);

		return *this;
	}
	BufferGeometry& BufferGeometry::translate(const float x, const float y, const float z)
	{
		_m1.makeTranslation(x, y, z);

		this->applyMatrix4(_m1);

		return *this;
	}
	BufferGeometry& BufferGeometry::scale(const float x, const float y, const float z)
	{
		_m1.makeScale(x, y, z);

		this->applyMatrix4(_m1);

		return *this;
	}
	BufferGeometry& BufferGeometry::lookAt(const Vector3& vector)
	{
		_obj.lookAt(vector);

		_obj.updateMatrix();

		this->applyMatrix4(_obj.matrix);

		return *this;
	}
	BufferGeometry& BufferGeometry::center()
	{
		this->computeBoundingBox();

		boundingBox.getCenter(&_offset); 
		_offset.negate();

		this->translate(_offset.x, _offset.y, _offset.z);

		return *this;
	}
	BufferGeometry& BufferGeometry::setFromObject(const Object3D& object)
	{
		auto geometry = object.geometry;

		if (object.isPoints || object.isLine) {

			auto positions = BufferAttribute<float>::create(geometry->vertices.size() * 3, 3);
			positions->copyVector3sArray(geometry->vertices);

			auto colors = BufferAttribute<float>::create(geometry->colors.size() * 3, 3);
			colors->copyColor3sArray(geometry->colors);			

			this->setAttribute(AttributeName::position,positions);
			//setAttribute<float>("position", positions);
			
			this->setAttribute(AttributeName::color,colors);			
			//setAttribute<float>("color", colors);

			if (geometry->lineDistances.size()>0 && geometry->lineDistances.size() ==  geometry->vertices.size()) {

				auto lineDistance = BufferAttribute<float>::create(geometry->lineDistances.size(),1);
				lineDistance->copyArray(&geometry->lineDistances[0],geometry->lineDistances.size());

				this->setAttribute(AttributeName::lineDistance,lineDistance);
				//setAttribute<float>("lineDistance", lineDistances);
			}

			//always not null
			//if (!geometry->boundingSphere.isEmpty()) {

				//boundingSphere = geometry->boundingSphere.clone(&boundingSphere);
				boundingSphere.copy(geometry->boundingSphere);

			//}

			//if (!geometry.boundingBox != = null) {

				//boundingBox = geometry->boundingBox.clone(&boundingBox);
				boundingBox.copy(geometry->boundingBox);

			//}

		}
		else if (object.isMesh) {

			if (geometry && geometry->isGeometry) {

				fromGeometry(*geometry);

			}

		}
		return *this;
	}
	BufferGeometry& BufferGeometry::setFromPoints(const std::vector<Vector3>&points)
	{
		/*List<float> position = new List<float>();


		for (int i = 0; i < points.Length; i++)
		{
			Vector3 point = points[i];

			position.Add(point.X);
			position.Add(point.Y);
			position.Add(point.Z);
		}*/

		auto position = BufferAttribute<float>::create(points.size() * 3, 3);
		position->copyVector3sArray(points);

		this->setAttribute(AttributeName::position,position);
		//setAttribute<float>("position", position);

		return *this;
	}
	BufferGeometry& BufferGeometry::updateFromObject(const Object3D& object)
	{
		auto geometry = object.geometry;

		if (object.isMesh) {

			auto direct = geometry->__directGeometry;

			if (geometry->elementsNeedUpdate == true) {
				direct.reset();
				direct = nullptr;
				geometry->elementsNeedUpdate = false;

			}

			if (direct == nullptr) {

				return this->fromGeometry(*geometry);

			}

			direct->verticesNeedUpdate = geometry->verticesNeedUpdate;
			direct->normalsNeedUpdate = geometry->normalsNeedUpdate;
			direct->colorsNeedUpdate = geometry->colorsNeedUpdate;
			direct->uvsNeedUpdate = geometry->uvsNeedUpdate;
			direct->groupsNeedUpdate = geometry->groupsNeedUpdate;

			geometry->verticesNeedUpdate = false;
			geometry->normalsNeedUpdate = false;
			geometry->colorsNeedUpdate = false;
			geometry->uvsNeedUpdate = false;
			geometry->groupsNeedUpdate = false;

			geometry = direct;		

		}


		if (geometry->verticesNeedUpdate == true) {

			auto attribute = getAttribute(AttributeName::position);

			if (attribute != nullptr) {

				attribute->copyVector3sArray(geometry->vertices);
				attribute->setNeedsUpdate(true);

			}

			geometry->verticesNeedUpdate = false;

		}

		if (geometry->normalsNeedUpdate ==  true) {

			auto attribute = getAttribute(AttributeName::normal);

			if (attribute != nullptr) {

				attribute->copyVector3sArray(geometry->normals);
				attribute->setNeedsUpdate(true);

			}

			geometry->normalsNeedUpdate = false;

		}

		if (geometry->colorsNeedUpdate == true) {

			auto attribute = getAttribute(AttributeName::color);

			if (attribute != nullptr) {

				attribute->copyColor3sArray(geometry->colors);
				attribute->setNeedsUpdate(true);

			}

			geometry->colorsNeedUpdate = false;

		}

		if (geometry->uvsNeedUpdate) {

			auto attribute = getAttribute(AttributeName::uv);

			if (attribute != nullptr) {

				attribute->copyVector2sArray(geometry->uvs);
				attribute->setNeedsUpdate(true);

			}

			geometry->uvsNeedUpdate = false;

		}

		if (geometry->lineDistancesNeedUpdate) {

			auto attribute = getAttribute(AttributeName::lineDistance);

			if (attribute != nullptr) {

				attribute->copyArray(&geometry->lineDistances[0],geometry->lineDistances.size());
				attribute->setNeedsUpdate(true);

			}

			geometry->lineDistancesNeedUpdate = false;

		}

		if (geometry->groupsNeedUpdate) {	

			auto directGeometry = reinterpret_cast<DirectGeometry*>(geometry.get());

			directGeometry->computeGroups(*object.geometry);

			this->groups = directGeometry->groups;

			directGeometry->groupsNeedUpdate = false;

		}

		return *this;
	}

	BufferGeometry& BufferGeometry::fromGeometry(Geometry& geometry)
	{
		if (geometry.__directGeometry) geometry.__directGeometry.reset();

		geometry.__directGeometry = std::make_shared<DirectGeometry>();
		geometry.__directGeometry->fromGeometry(geometry);

		return this->fromDirectGeometry(*geometry.__directGeometry);
	}

	BufferGeometry& BufferGeometry::fromDirectGeometry(DirectGeometry& geometry)
	{
		//float[] positions = new float[geometry.Vertices.Count * 3];

		//std::vector<float> _positions(geometry.vertices.size() * 3);
		auto _positions = BufferAttribute<float>::create(geometry.vertices.size() * 3, 3);
		_positions->copyVector3sArray(geometry.vertices);
		setAttribute(AttributeName::position,_positions);
		//setAttribute<float>("position", _positions);
		//this.SetAttribute("position", new BufferAttribute<float>(positions, 3).CopyVector3sArray(geometry.Vertices.ToArray()));

		if (geometry.normals.size() > 0)
		{
			auto _normals = BufferAttribute<float>::create(geometry.normals.size() * 3, 3);
			_normals->copyVector3sArray(geometry.normals);
			setAttribute(AttributeName::normal,_normals);
			//setAttribute<float>("normal", _normals);

			//float[] normals = new float[geometry.Normals.Count * 3];
			//this.SetAttribute("normal", new BufferAttribute<float>(normals, 3).CopyVector3sArray(geometry.Normals.ToArray()));
		}

		if (geometry.colors.size() > 0)
		{
			auto _colors = BufferAttribute<float>::create(geometry.colors.size() * 3, 3);
			_colors->copyColor3sArray(geometry.colors);
			setAttribute(AttributeName::color,_colors);
			//setAttribute<float>("color", _colors);
			//float[] colors = new float[geometry.Colors.Count * 3];
			//this.SetAttribute("color", new BufferAttribute<float>(colors, 3).CopyColorsArray(geometry.Colors.ToArray()));
		}

		if (geometry.uvs.size() > 0)
		{
			auto _uvs = BufferAttribute<float>::create(geometry.uvs.size() * 2, 2);
			_uvs->copyVector2sArray(geometry.uvs);
			setAttribute(AttributeName::uv,_uvs);
			//setAttribute<float>("uv", _uvs);
			//float[] uvs = new float[geometry.uvs.Count * 2];
			//this.SetAttribute("uv", new BufferAttribute<float>(uvs, 2).CopyVector2sArray(geometry.uvs.ToArray()));
		}

		if (geometry.uvs2.size() > 0)
		{
			auto _uvs2 = BufferAttribute<float>::create(geometry.uvs2.size() * 2, 2);
			_uvs2->copyVector2sArray(geometry.uvs2);
			setAttribute(AttributeName::uv2,_uvs2);
			//setAttribute<float>("uv2", _uvs2);
			//float[] uvs2 = new float[geometry.uvs2.Count * 2];
			//this.SetAttribute("uv2", new BufferAttribute<float>(uvs2, 2).CopyVector2sArray(geometry.uvs2.ToArray()));
		}

		// groups
		this->groups = geometry.groups;

		// morphPosition
				
		for (auto& it : geometry.morphPositionNormal) {
			std::vector<BufferAttribute<float>::ptr> array;
			
			std::vector<MorphTarget> morphTargets = it.second;
			for (unsigned int i = 0; i < morphTargets.size(); i++) {
				auto morphTarget = morphTargets[i];
				auto attribute = BufferAttribute<float>::create(morphTarget.vertices.size() * 3, 3);
				attribute->copyVector3sArray(morphTarget.vertices);
				attribute->name = morphTarget.name;
				array.push_back(attribute);
			}
			this->morphAttributes[it.first]=array;
			
		}		

		//skinning

		if (geometry.skinIndices.size() > 0)
		{
			auto skinBuffer = BufferAttribute<float>::create(geometry.skinIndices.size() * 4, 4);
			skinBuffer->copyVector4sArray(geometry.skinIndices);

			this->setAttribute(AttributeName::skinIndex,skinBuffer);
			//setAttribute<float>("skinIndex", skinBuffer);

			//float[] skinBuffer = new float[geometry.SkinIndices.Count * 4];
			//var skinIndices = new BufferAttribute<float>(skinBuffer, 4);
			//this.SetAttribute("skinIndex", skinIndices.CopyVector4sArray(geometry.SkinIndices.ToArray()));
		}

		if (geometry.skinWeights.size() > 0)
		{
			auto skinBuffer = BufferAttribute<float>::create(geometry.skinWeights.size() * 4, 4);
			skinBuffer->copyVector4sArray(geometry.skinWeights);
			this->setAttribute(AttributeName::skinWeight,skinBuffer);
			//setAttribute<float>("skinWeight", skinBuffer);

			/*float[] skinBuffer = new float[geometry.SkinWeights.Count * 4];
			var skinWeights = new BufferAttribute<float>(skinBuffer, 4);
			this.SetAttribute("skinWeight", skinWeights.CopyVector4sArray(geometry.SkinWeights.ToArray()));*/
		}

		//if (geometry.BoundingSphere != null)
		//{
		boundingSphere = geometry.boundingSphere.clone(&boundingSphere);
		//}

		//if (geometry.BoundingBox != null)
		//{
		boundingBox = geometry.boundingBox.clone(&boundingBox);
		//}
		return *this;
	}
	void BufferGeometry::computeBoundingBox()
	{
		/*if (this.BoundingBox == null)
		{
			this.BoundingBox = new Box3();
		}*/

		auto position = getAttribute(AttributeName::position);
		//var morphAttributesPosition = this.MorphAttributes["position"] as List<BufferAttribute<float>>;

		if (position != nullptr)
		{
			boundingBox.setFromBufferAttribute(*position);

			// process morph attributes if present

			if (morphAttributes.size()>0)
			{
				for (auto& morph: morphAttributes)
				{
					std::vector<BufferAttribute<float>::ptr> morphAttributelist = morphAttributes[morph.first];
					for (unsigned int i = 0; i < morphAttributelist.size(); i++) {
						_box.setFromBufferAttribute(*morphAttributelist[i]);
						boundingBox.expandByPoint(_box.min);
						boundingBox.expandByPoint(_box.max);
					}
				}
			}
		}
		else
		{
			boundingBox.makeEmpty();
		}

		if (std::isnan(boundingBox.min.x) || std::isnan(boundingBox.min.y) || std::isnan(boundingBox.min.z))
		{
			//TODO
			//Trace.TraceError("THREE.Core.BufferGeometry.ComputeBoundingBox : Compute min/max have Nan values. The \"Position\" attribute is likely to have NaN values.");
		}
	}
	void BufferGeometry::computeBoundingSphere()
	{
		/*if (this.BoundingSphere == null)
		{
			this.BoundingSphere = new Sphere();
		}*/

		/*BufferAttribute<float> position = null;

		if (this.Attributes.ContainsKey("position"))
			position = (BufferAttribute<float>)this.Attributes["position"];

		List<BufferAttribute<float>> morphAttributesPosition = null;
		if (this.MorphAttributes.ContainsKey("position"))
			morphAttributesPosition = this.MorphAttributes["position"] as List<BufferAttribute<float>>;*/

		auto position = getAttribute(AttributeName::position);

		if (position != nullptr)
		{
			auto center = boundingSphere.center;

			_box.setFromBufferAttribute(*position);

			if (morphAttributes.size()>0)
			{
				for (auto& morph : morphAttributes)
				{
					std::vector<BufferAttribute<float>::ptr> morphAttributelist = morphAttributes[morph.first];
					for (unsigned int i = 0; i < morphAttributelist.size(); i++) {
						_boxMorphTargets.setFromBufferAttribute(*morphAttributelist[i]);
						_box.expandByPoint(_boxMorphTargets.min);
						_box.expandByPoint(_boxMorphTargets.max);
					}
				}				
			}

			center = _box.getCenter(&center);
			boundingSphere.center = center;

			// second, try to find a boundingSphere with a radius smaller than the
			// boundingSphere of the boundingBox: sqrt(3) smaller in the best case

			float maxRadiusSq = 0;

			for (unsigned int i = 0; i < position->count; i++)
			{
				_vector = _vector.fromBufferAttribute(*position, i);
				maxRadiusSq = std::max(maxRadiusSq, center.distanceToSquared(_vector));
			}

			// process morph attributes if present
			if (morphAttributes.size()>0)
			{
				for (auto& morph : morphAttributes)
				{
					std::vector<BufferAttribute<float>::ptr> morphAttributelist = morphAttributes[morph.first];
					for (unsigned int i = 0; i < morphAttributelist.size(); i++) {
						for (unsigned int j = 0; j < morphAttributelist[i]->size; j++) {
							_vector = _vector.fromBufferAttribute(*morphAttributelist[i], i);
							maxRadiusSq = std::max(maxRadiusSq, center.distanceToSquared(_vector));							
						}
					}
				}				
			}

			boundingSphere.radius = (float)sqrt(maxRadiusSq);

			if (std::isnan(boundingSphere.radius))
			{
				//TODO
				//Trace.TraceError("THREE.Core.BufferGeometry.ComputeBoundingSphere():Computed radius is Nan. The 'Position' attribute is likely to hava Nan values.");
			}
		}
	}
	void BufferGeometry::computeVertexNormals(bool areaWeighted)
	{
		auto position = getAttribute(AttributeName::position);
		if (position != nullptr)
		{
			auto positionArray = position->array;
			auto normal = getAttribute(AttributeName::normal);
			if (normal==nullptr)
			{
				std::vector<float> normal_array(position->size, 0.0f);
				auto normalAttribute = BufferAttribute<float>::create(normal_array, 3);
				setAttribute(AttributeName::normal,normalAttribute);
				//setAttribute<float>("normal", normalAttribute);
				normal = normalAttribute;
			}
			else
			{		
				for (unsigned int i = 0; i < normal->size; i++)
				{
					normal->array[i] = 0;
				}
			}

			//auto normalArray = &normal->array;

			int vA, vB, vC;
			vA = vB = vC = 0;

			Vector3 pA;
			Vector3 pB;
			Vector3 pC;
			Vector3 cb;
			Vector3 ab;

			if (index != nullptr)
			{
				auto indices = index->array;

				for (unsigned int i = 0; i < index->count; i += 3)
				{
					vA = indices[i + 0] * 3;
					vB = indices[i + 1] * 3;
					vC = indices[i + 2] * 3;

					pA = Vector3().fromArray(positionArray, vA);
					pB = Vector3().fromArray(positionArray, vB);
					pC = Vector3().fromArray(positionArray, vC);

					cb.subVectors(pC, pB);
					ab.subVectors(pA, pB);

					cb.cross(ab);
					if ((vA + 2) < normal->array.size()) {
						normal->array[vA] += cb.x;
						normal->array[vA + 1] += cb.y;
						normal->array[vA + 2] += cb.z;
					}
					if ((vB + 2) < normal->array.size()) {
						normal->array[vB] += cb.x;
						normal->array[vB + 1] += cb.y;
						normal->array[vB + 2] += cb.z;
					}
					if ((vC + 2) < normal->array.size()) {
						normal->array[vC] += cb.x;
						normal->array[vC + 1] += cb.y;
						normal->array[vC + 2] += cb.z;
					}
				}
			}
			else
			{
				//auto pLen = position->size - 1;
				unsigned size = normal->array.size();

				for (unsigned int i = 0; i < position->size; i += 9)
				{
					pA = Vector3().fromArray(positionArray, i);
					pB = Vector3().fromArray(positionArray, i + 3);					
					pC = Vector3().fromArray(positionArray, i + 6);

					cb = pC - pB;
					ab = pA - pB;
					cb.cross(ab);			

					if(i<size)
					normal->array[i] = cb.x;
					if((i+1)<size)
					normal->array[i + 1] = cb.y;

					if ((i + 2) < size)
					normal->array[i + 2] = cb.z;

					if ((i + 3) < size)
					normal->array[i + 3] = cb.x;

					if ((i + 4) < size)
					normal->array[i + 4] = cb.y;

					if ((i + 5) < size)
					normal->array[i + 5] = cb.z;

					if ((i + 6) < size)
					normal->array[i + 6] = cb.x;

					if ((i + 7) < size)
					normal->array[i + 7] = cb.y;

					if ((i + 8) < size)
					normal->array[i + 8] = cb.z;
				}
				
			}

			normalizeNormals();

			normal->setNeedsUpdate(true);
		}
	}
	
	const BufferAttribute<unsigned>::ptr& BufferGeometry::mergeAttributeInt(const BufferAttribute<unsigned>::ptr& source, const BufferAttribute<unsigned>::ptr& target, int offset)
	{
		auto positionArray1 = &source->array;
		auto positionArray2 = &target->array;

		if (positionArray1->size()==0 && positionArray2->size()==0) return source;

		auto positionOffset = target->itemSize * offset;
		auto length = std::min(target->size, source->size - positionOffset);

		for (unsigned int i = 0, j = positionOffset; i < length; i++, j++) {
			positionArray1->at(j) = positionArray2->at(i);
		}

		return source;
	}

	const BufferAttribute<float>::ptr& BufferGeometry::mergeAttribute(const BufferAttribute<float>::ptr& source, const BufferAttribute<float>::ptr& target, int offset)
	{
		auto positionArray1 = &source->array;
		auto positionArray2 = &target->array;

		if (positionArray1->size()==0 && positionArray2->size()==0) return source;

		auto positionOffset = target->itemSize * offset;
		auto length = std::min(target->size, source->size - positionOffset);

		for (unsigned int i = 0, j = positionOffset; i < length; i++, j++) {
			positionArray1->at(j) = positionArray2->at(i);
		}

		return source;
	}

	BufferGeometry& BufferGeometry::merge(BufferGeometry& geometry, int offset)
	{
		index = mergeAttributeInt(index, geometry.index, offset);
		auto position = getAttribute(AttributeName::position);
		position = mergeAttribute(position, geometry.getAttribute(AttributeName::position), offset);

		auto normal = getAttribute(AttributeName::normal);
		normal = mergeAttribute(normal, geometry.getAttribute(AttributeName::normal), offset);

		auto color = getAttribute(AttributeName::color);
		color = mergeAttribute(color, geometry.getAttribute(AttributeName::color), offset);

		auto uv = getAttribute(AttributeName::uv);
		uv = mergeAttribute(uv, geometry.getAttribute(AttributeName::uv), offset);

		auto uv2 = getAttribute(AttributeName::uv2);
		uv2 = mergeAttribute(uv2, geometry.getAttribute(AttributeName::uv2), offset);

		auto tangent = getAttribute(AttributeName::tangent);
		tangent = mergeAttribute(tangent, geometry.getAttribute(AttributeName::tangent), offset);

		auto bitangent = getAttribute(AttributeName::bitangent);
		bitangent = mergeAttribute(bitangent, geometry.getAttribute(AttributeName::bitangent), offset);

		auto lineDistance = getAttribute(AttributeName::lineDistance);
		lineDistance = mergeAttribute(lineDistance, geometry.getAttribute(AttributeName::lineDistance), offset);

		for (auto& attr : geometry.morphAttributes) {
			std::string key = attr.first;
			std::vector<BufferAttribute<float>::ptr> vec = attr.second;

			morphAttributes.insert({ key,vec });
		}

		//std::copy(geometry.morphAttributes.begin(), geometry.morphAttributes.end(), morphAttributes.end());
		auto skinIndexAttribute = getAttribute(AttributeName::skinIndex);
		skinIndexAttribute = mergeAttribute(skinIndexAttribute, geometry.getAttribute(AttributeName::skinIndex), offset);

		auto skinWeightAttribute = getAttribute(AttributeName::skinWeight);
		skinWeightAttribute = mergeAttribute(skinWeightAttribute, geometry.getAttribute(AttributeName::skinWeight), offset);

		/*{
			auto positionArray1 = position->array;
			auto positionArray2 = geometry.position->array;

			auto positionOffset = geometry.position->itemSize * offset;
			auto length = std::min(geometry.position->size, position->size - positionOffset);

			for (int i = 0, j = positionOffset; i < length; i++, j++) {
				positionArray1[j] = positionArray2[i];
			}
		}*/


		return *this;
	}
	void BufferGeometry::normalizeNormals()
	{
		//BufferAttribute<float> normals = (BufferAttribute<float>)this.Attributes["normal"];
		auto normal = getAttribute(AttributeName::normal);
		for (unsigned int i = 0; i < normal->count; i++)
		{
			_vector.x = normal->getX(i);
			_vector.y = normal->getY(i);
			_vector.z = normal->getZ(i);

			_vector.normalize();

			normal->setXYZ(i, _vector.x, _vector.y, _vector.z);
		}
	}
	void BufferGeometry::computeFaceNormals()
	{
		//backwards compatibility
	}
	BufferGeometry* BufferGeometry::clone()
	{
		return new BufferGeometry(*this);
	}
	BufferGeometry& BufferGeometry::copy(const BufferGeometry& source)
	{
		Geometry::copy(source);

		name = source.name;

		if (source.index) index.reset(source.index->clone());

		for (auto attr = source.attributes.begin();attr != source.attributes.end();) {
			BufferAttribute<float>::ptr newAttr = std::make_shared<BufferAttribute<float>>();
			newAttr.reset(attr->second->clone());
			attributes[attr->first] = newAttr;
			attr++;

		}

		/*if (source.position) position.reset(source.position->clone());

		if (source.normal) normal.reset(source.normal->clone());

		if (source.color) color.reset(source.color->clone());

		if (source.uv) uv.reset(source.uv->clone());

		if (source.uv2) uv2.reset(source.uv2->clone());

		if (source.tangent) tangent.reset(source.tangent->clone());

		if (source.bitangent) bitangent.reset(source.bitangent->clone());

		if (source.lineDistance) lineDistance.reset(source.lineDistance->clone());*/				

		drawCalls.assign(source.drawCalls.begin(), source.drawCalls.end());
		offsets.assign(source.offsets.begin(), source.offsets.end());

		morphAttributes.clear();
		for (auto& attr : source.morphAttributes) {
			std::vector<BufferAttribute<float>::ptr> array;
			for (auto ptr : attr.second) {
				auto positionPtr = BufferAttribute<float>::create();
				positionPtr->copy(*ptr);
				array.push_back(positionPtr);
			}
			morphAttributes.insert({ attr.first,array });
		}

		/*if (source.skinIndex) skinIndex.reset(source.skinIndex->clone());

		if (source.skinWeight) skinWeight.reset(source.skinWeight->clone());*/

		morphTargetsRelative = source.morphTargetsRelative;

		drawRange = source.drawRange;

		groups.assign(source.groups.begin(), source.groups.end());			
			

		return *this;
	}

	void BufferGeometry::toNonIndexed(const BufferGeometry::ptr& geometry2)
	{
		if (index == nullptr)
		{
			//Trace.TraceError("THREE.Core.BufferGeometry.ToNonIndexed:Geometry is already non-indexed.");
			return;
		}

		

		std::vector<unsigned> indices = this->index->array;
		//auto attr = attributes;

		/*index = mergeAttributeInt(index, geometry2->index, offset);
		position = mergeAttribute<float>(position, geometry.position, offset);
		normal = mergeAttribute<float>(normal, geometry.normal, offset);
		color = mergeAttribute<float>(color, geometry.color, offset);
		uv = mergeAttribute<float>(uv, geometry.uv, offset);
		uv2 = mergeAttribute<float>(uv2, geometry.uv2, offset);
		tangent = mergeAttribute<float>(tangent, geometry.tangent, offset);
		bitangent = mergeAttribute<float>(bitangent, geometry.bitangent, offset);
		lineDistance = mergeAttribute<float>(lineDistance, geometry.lineDistance, offset);
		std::copy(geometry.morphAttributes.begin(), geometry.morphAttributes.end(), morphAttributes.end());

		skinIndexAttribute = mergeAttribute<float>(skinIndexAttribute, geometry.skinIndexAttribute, offset);
		skinWeightAttribute = mergeAttribute<float>(skinWeightAttribute, geometry.skinWeightAttribute, offset);*/

		if (index != nullptr) {
			auto newIndex = BufferAttribute<unsigned>::create();
			newIndex->convertBufferAttributeToNonIndexed(*index, indices);
			geometry2->setIndex(newIndex);
		}

		for (auto it = attributes.begin();it != attributes.end();) {
			auto newAttribute = BufferAttribute<float>::create();
			newAttribute->convertBufferAttributeToNonIndexed(*attributes[it->first], indices);
			geometry2->attributes.insert({ it->first, newAttribute });
			it++;
		}

		/*
		if (position != nullptr) {
			auto newPosition = BufferAttribute<float>::create();
			newPosition->convertBufferAttributeToNonIndexed(*position, indices);
			geometry2->setPosition(newPosition);
		}
		if (normal != nullptr) {
			auto newNormal = BufferAttribute<float>::create();
			newNormal->convertBufferAttributeToNonIndexed(*normal, indices);
			geometry2->setNormal(newNormal);
		}

		if (color != nullptr) {
			auto newColor = BufferAttribute<float>::create();
			newColor->convertBufferAttributeToNonIndexed(*color, indices);
			geometry2->setColor(newColor);
		}

		if (uv != nullptr) {
			auto newUv = BufferAttribute<float>::create();
			newUv->convertBufferAttributeToNonIndexed(*uv, indices);
			geometry2->setUV(newUv);
		}

		if (uv2 != nullptr) {
			auto newUv2 = BufferAttribute<float>::create();
			newUv2->convertBufferAttributeToNonIndexed(*uv2, indices);
			geometry2->setUV2(newUv2);
		}

		if (tangent != nullptr) {
			auto newTangent = BufferAttribute<float>::create();
			newTangent->convertBufferAttributeToNonIndexed(*tangent, indices);
			geometry2->setTangent(newTangent);
		}

		if (bitangent != nullptr) {
			auto newBitangent = BufferAttribute<float>::create();
			newBitangent->convertBufferAttributeToNonIndexed(*bitangent, indices);
			geometry2->setBitangent(newBitangent);
		}

		if (skinIndex != nullptr) {
			auto newSkinIndex = BufferAttribute<float>::create();
			newSkinIndex->convertBufferAttributeToNonIndexed(*skinIndex, indices);
			geometry2->setSkinIndexAttribute(newSkinIndex);
		}

		if (skinWeight != nullptr) {
			auto newSkinWeight = BufferAttribute<float>::create();
			newSkinWeight->convertBufferAttributeToNonIndexed(*skinWeight, indices);
			geometry2->setSkinWeightAttribute(newSkinWeight);
		}

		if (lineDistance != nullptr) {
			auto newLineDistance = BufferAttribute<float>::create();
			newLineDistance->convertBufferAttributeToNonIndexed(*lineDistance, indices);
			geometry2->setLineDistance(newLineDistance);
		}*/
		

		//auto morphAttributes = this->.morphAttributes;

		
		for (auto& attr : morphAttributes) {
			std::string key = attr.first;

			std::vector<BufferAttribute<float>::ptr> array;
			for (auto ptr : attr.second) {
				BufferAttribute<float>::ptr newPtr = BufferAttribute<float>::create();
				newPtr->copy(*ptr);
				array.push_back(newPtr);
			}
			geometry2->morphAttributes.insert({ key,array });
		}

		/*foreach(string name in MorphAttributes.Keys)
		{
			List<BufferAttribute<float>> morphArray = new List<BufferAttribute<float>>();
			BufferAttribute<float>[] morphAttribute = (BufferAttribute<float>[])morphAttributes[name];

			for (int i = 0; i < morphAttribute.Length; i++)
			{
				BufferAttribute<float> attribute = morphAttribute[i];
				var newAttribute = ConvertBufferAttribute(attribute, indices);
				morphArray.Add(newAttribute);
			}

			geometry2.MorphAttributes.Add(name, morphArray);
		}*/

		//var groups = this.Groups;

		std::copy(groups.begin(), groups.end(), geometry2->groups.begin());

		geometry2->groups.assign(groups.begin(), groups.end());
		/*for (unsigned i = 0; i < groups.size(); i++)
		{
			autovar group = groups[i];
			geometry2.AddGroup((int)group.Start, (int)group.Count, group.MaterialIndex);
		}*/

		
	}
}