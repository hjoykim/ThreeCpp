#include "Object3D.h"
#include <three/cameras/Camera.h>
#include <three/scenes/Scene.h>
#include <three/objects/Group.h>
#include <three/renderers/GLRenderer.h>
#include <iostream>
namespace three {

	namespace object3d {		

		Vector3 _v1;
		Quaternion _q1;
		Matrix4 _m1;
		Vector3 _target;

		Vector3 _position;
		Vector3 _scale;
		Quaternion _quaternion;

		Vector3 _xAxis(1, 0, 0);
		Vector3 _yAxis(0, 1, 0);
		Vector3 _zAxis(0, 0, 1);
	}
	using namespace object3d;

	Object3D::Object3D() :id(++_objectIdCount) {
		rotation.onChange.connect(*this, &Object3D::onRotationChange);
		quaternion.onChange.connect(*this, &Object3D::onQuaternionChange);	

		this->geometry = BufferGeometry::create();
		this->material = MeshBasicMaterial::create();
		material->color.set(0xffffff);
	}

	Object3D::Object3D(const Geometry::ptr& geometry, const Material::ptr& material) : id(++_objectIdCount),  geometry(geometry), material(material),materials({ material }) {
		rotation.onChange.connect(*this, &Object3D::onRotationChange);
		quaternion.onChange.connect(*this, &Object3D::onQuaternionChange);	
	}

	Object3D::Object3D(const Geometry::ptr& geometry, const std::initializer_list<Material::ptr>& materials) : id(++_objectIdCount),geometry(geometry),materials(materials)
	{
		rotation.onChange.connect(*this, &Object3D::onRotationChange);
		quaternion.onChange.connect(*this, &Object3D::onQuaternionChange);

		if (geometry != nullptr)
			this->geometry = geometry;

		if (materials.size() > 0) {
			this->materials.resize(materials.size());
			std::copy(materials.begin(), materials.end(), this->materials.begin());
		}
		this->material = this->materials[0];
	}

	Object3D::Object3D(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials) : id(++_objectIdCount), geometry(geometry), material(material), materials({ material }) {
	
		rotation.onChange.connect(*this, &Object3D::onRotationChange);
		quaternion.onChange.connect(*this, &Object3D::onQuaternionChange);

		if (geometry != nullptr)
			this->geometry = geometry;

		if (materials.size() > 0) {
			this->materials.resize(materials.size());
			std::copy(materials.begin(), materials.end(), this->materials.begin());
		}
		this->material = this->materials[0];
	}

	Object3D::Object3D(const Object3D& object) : Object3D() {
		name = object.name;
		up.copy(object.up);
		position.copy(object.position);
		rotation.copy(object.rotation);
		quaternion.copy(object.quaternion);
		scale.copy(object.scale);
		matrix.copy(object.matrix);
		modelViewMatrix.copy(object.modelViewMatrix);
		normalMatrix.copy(object.normalMatrix);
		matrixWorld.copy(object.matrixWorld);
		matrixAutoUpdate = object.matrixAutoUpdate;
		matrixWorldNeedsUpdate = object.matrixWorldNeedsUpdate;
		layers.copy(object.layers);
		visible = object.visible;
		castShadow = object.castShadow;
		receiveShadow = object.receiveShadow;
		frustumCulled = object.frustumCulled;
		renderOrder = object.renderOrder;
		if (children.size() != object.children.size()) {
			children.resize(object.children.size());
		}
		std::copy(object.children.begin(), object.children.end(), children.begin());
		
		if (geometry == nullptr && object.geometry != nullptr) {
			geometry = BufferGeometry::create();
			geometry->copy(*object.geometry);
		}
		if (materials.size() != object.materials.size()) {
			materials.resize(object.materials.size());
			std::copy(object.materials.begin(), object.materials.end(), materials.begin());
			material = object.materials[0];
		}

		
	}

	void Object3D::onRotationChange(const math::Euler& rotation)
	{
		quaternion.setFromEuler(this->rotation, false);
	}

	void Object3D::onQuaternionChange(const math::Quaternion& quaternion)
	{
		rotation.setFromQuaternion(this->quaternion, Euler::RotationOrder::Default, false);
	}

	void Object3D::applyMatrix4(math::Matrix4& matrix)
	{
		if (matrixAutoUpdate) updateMatrix();

		matrix.premultiply(matrix);

		matrix.decompose(&position, &quaternion, &scale);

	}

	Object3D& Object3D::applyQuaternion(math::Quaternion& quaternion)
	{
		quaternion.premultiply(quaternion);

		return *this;
	}

	void Object3D::setRotationFromAxisAngle(math::Vector3& axis, float angle)
	{
		quaternion.setFromAxisAngle(axis, angle);
	}

	void Object3D::setRotationFromEuler(math::Euler& euler)
	{
		quaternion.setFromEuler(euler, true);
	}

	void Object3D::setRotationFromMatrix(math::Matrix4& m)
	{
		// assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

		quaternion.setFromRotationMatrix(m);
	}

	void Object3D::setRotationFromQuaternion(math::Quaternion& q)
	{
		// assumes q is normalized

		quaternion.copy(q);
	}

	Object3D& Object3D::rotateOnAxis(const math::Vector3& axis, float angle)
	{
		// TODO: 여기에 return 문을 삽입합니다.
		// rotate object on axis in object space
		// axis is assumed to be normalized

		_q1.setFromAxisAngle(axis, angle);

		quaternion.multiply(_q1);

		return *this;
	}

	Object3D& Object3D::rotateOnWorldAxis(const math::Vector3& axis, float angle)
	{
		// TODO: 여기에 return 문을 삽입합니다.
		// rotate object on axis in world space
		// axis is assumed to be normalized
		// method assumes no rotated parent

		_q1.setFromAxisAngle(axis, angle);

		quaternion.premultiply(_q1);

		return *this;
	}

	Object3D& Object3D::rotateX(float angle)
	{
		return this->rotateOnAxis(_xAxis, angle);
	}

	Object3D& Object3D::rotateY(float angle)
	{
		return this->rotateOnAxis(_yAxis, angle);
	}

	Object3D& Object3D::rotateZ(float angle)
	{
		return this->rotateOnAxis(_zAxis, angle);
	}

	Object3D& Object3D::translateOnAxis(const math::Vector3& axis, float distance)
	{
		// TODO: 여기에 return 문을 삽입합니다.
		// translate object by distance along axis in object space
		// axis is assumed to be normalized

		_v1.copy(axis).applyQuaternion(quaternion);

		position.add(_v1.multiplyScalar(distance));

		return *this;
	}

	Object3D& Object3D::translateX(float distance)
	{
		return translateOnAxis(_xAxis, distance);
	}

	Object3D& Object3D::translateY(float distance)
	{
		// TODO: 여기에 return 문을 삽입합니다.
		return translateOnAxis(_yAxis, distance);
	}

	Object3D& Object3D::translateZ(float distance)
	{
		// TODO: 여기에 return 문을 삽입합니다.
		return translateOnAxis(_zAxis, distance);
	}

	Vector3 Object3D::localToWorld(math::Vector3& vector) 
	{
		// TODO: 여기에 return 문을 삽입합니다.
		return vector.applyMatrix4(matrixWorld);
	}

	Vector3 Object3D::worldToLocal(math::Vector3& vector)
	{
		// TODO: 여기에 return 문을 삽입합니다.
		return vector.applyMatrix4(_m1.getInverse(matrixWorld));
	}

	void Object3D::lookAt(const math::Vector3& vector)
	{
		_target.copy(vector);
		//var parent = this.parent;

		updateWorldMatrix(true, false);

		_position.setFromMatrixPosition(matrixWorld);

		if (isCamera || isLight) {

			_m1.lookAt(_position, _target, up);

		}
		else {

			_m1.lookAt(_target, _position, up);

		}

		quaternion.setFromRotationMatrix(_m1);

		if (parent) {

			_m1.extractRotation(parent->matrixWorld);
			_q1.setFromRotationMatrix(_m1);
			quaternion.premultiply(_q1.inverse());

		}

	}

	void Object3D::lookAt(float x, float y, float z)
	{
		Vector3 vector(x, y, z);
		lookAt(vector);
		/*_target.set(x, y, z);

		updateWorldMatrix(true, false);

		_position.setFromMatrixPosition(matrixWorld);

		if (isCamera || isLight) {

			_m1.lookAt(_position, _target, up);

		}
		else {

			_m1.lookAt(_target, _position,up);

		}

		quaternion.setFromRotationMatrix(_m1);

		if (parent) {

			_m1.extractRotation(parent->matrixWorld);
			_q1.setFromRotationMatrix(_m1);
			quaternion.premultiply(_q1.inverse());
		}*/

	
	}

	Object3D& Object3D::add(const Object3D::ptr& object)
	{
		// TODO: 여기에 return 문을 삽입합니다.
		if (object.get()==this) return *this;
		
		
		if (object->parent) {
			object->parent->remove(object);
		}
		object->parent = this;
		children.push_back(object);
		
		return *this;
	}
		
	Object3D& Object3D::remove(const Object3D::ptr& object)
	{
		// TODO: 여기에 return 문을 삽입합니다.
		auto found = std::find(children.begin(), children.end(), object);

		if (found != children.end()) {

			(*found)->parent = nullptr;			

			children.erase(found);
			
		}

		return *this;
	}

	Object3D& Object3D::removeAll()
	{
		for (auto child : children) {
			child->parent = nullptr;
		}
		children.clear();

		return *this;
	}

	Object3D& Object3D::attach(const Object3D::ptr& object)
	{
		// adds object as a child of this, while maintaining the object's world transform

		updateWorldMatrix(true, false);

		_m1.getInverse(matrixWorld);

		if (object->parent) {

			object->parent->updateWorldMatrix(true, false);

			_m1.multiply(object->parent->matrixWorld);

		}

		object->applyMatrix4(_m1);

		object->updateWorldMatrix(false, false);

		add(object);

		return *this;
	}
	

	Object3D* Object3D::clone(bool recursive)
	{
		return new Object3D(*this);
	}

	Object3D& Object3D::copy(const Object3D& source,bool recursive)
	{

		//return new Object3D(source);
		name = source.name;
		up.copy(source.up);
		position.copy(source.position);
		rotation.copy(source.rotation);
		quaternion.copy(source.quaternion);
		scale.copy(source.scale);
		matrix.copy(source.matrix);
		modelViewMatrix.copy(source.modelViewMatrix);
		normalMatrix.copy(source.normalMatrix);
		matrixWorld.copy(source.matrixWorld);
		matrixAutoUpdate = source.matrixAutoUpdate;
		matrixWorldNeedsUpdate = source.matrixWorldNeedsUpdate;
		layers.copy(source.layers);
		visible = source.visible;
		castShadow = source.castShadow;
		receiveShadow = source.receiveShadow;
		frustumCulled = source.frustumCulled;
		renderOrder = source.renderOrder;
		if (children.size() != source.children.size()) {
			children.resize(source.children.size());
		}
		std::copy(source.children.begin(), source.children.end(), children.begin());
		geometry->copy(*source.geometry);

		if (materials.size() != source.materials.size()) {
			materials.resize(source.materials.size());
			std::copy(source.materials.begin(), source.materials.end(), materials.begin());
			material = source.materials[0];
		}	

		return *this;
	}

	math::Vector3& Object3D::getWorldPosition(math::Vector3& target)
	{
		updateMatrixWorld(true);

		float* e = matrixWorld.elements;

		return target.set(e[8], e[9], e[10]).normalize();
	}

	math::Quaternion& Object3D::getWorldQuaternion(math::Quaternion& target)
	{
		updateMatrixWorld(true);

		matrixWorld.decompose(&_position, &target, &_scale);

		return target;
	}

	math::Vector3& Object3D::getWorldScale(math::Vector3&& target)
	{
		updateMatrixWorld(true);

		matrixWorld.decompose(&_position, &_quaternion, &target);

		return target;
	}

	math::Vector3& Object3D::getWorldDirection(math::Vector3& target)
	{
		updateMatrixWorld(true);

		float* e = matrixWorld.elements;

		return target.set(e[8], e[9], e[10]).normalize();
	}

	void Object3D::traverse(std::function<void(Object3D&)> callback)
	{
		callback(*this);

		for (auto child : children) {
			child->traverse(callback);
		}
	}

	void Object3D::traverseVisible(std::function<void(Object3D&)> callback)
	{
		if (!visible) return;

		callback(*this);

		for (auto child : children) {
			child->traverseVisible(callback);
		}

	}

	void Object3D::traverseAncestors(std::function<void(Object3D&)> callback)
	{
		if (parent) {
			callback(*this);
			parent->traverseAncestors(callback);
		}
	}

	void Object3D::updateMatrix()
	{
		matrix.compose(position, quaternion, scale);

		matrixWorldNeedsUpdate = true;
	}

	void Object3D::updateMatrixWorld(bool force)
	{		
		if (matrixAutoUpdate) updateMatrix();

		if (matrixWorldNeedsUpdate || force) {

			if (parent == nullptr) {

				matrixWorld.copy(matrix);

			}
			else {

				matrixWorld.multiplyMatrices(parent->matrixWorld, matrix);

			}

			matrixWorldNeedsUpdate = false;

			force = true;

		}

		// update children
		for (auto child : children) {
			child->updateMatrixWorld(force);
		}
		

	}

	void Object3D::updateWorldMatrix(bool updateParents, bool updateChildren)
	{

		if (updateParents ==  true && parent) {

			parent->updateWorldMatrix(true, false);

		}

		if (matrixAutoUpdate) updateMatrix();

		if (parent == nullptr) {

			matrixWorld.copy(matrix);

		}
		else {

			matrixWorld.multiplyMatrices(parent->matrixWorld, matrix);

		}

		// update children

		if (updateChildren ==  true) {
			for (auto child : children) {
				child->updateWorldMatrix(false, true);
			}
		}
	}

}
