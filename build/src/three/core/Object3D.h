#ifndef THREE_OBJECT3D_H
#define THREE_OBJECT3D_H
#include <three/utils/osdecl.h>
#include <vector>
#include <memory>
#include <string>
#include <three/utils/simplesignal.h>
#include <three/math/Vector3.h>
#include <three/math/Matrix3.h>
#include <three/math/Matrix4.h>
#include <three/math/Euler.h>
#include <three/math/Quaternion.h>
#include <three/core/Geometry.h>
#include <three/materials/Material.h>
#include <three/utils/sole.h>
#include <three/core/Layers.h>
#include <three/core/BufferAttribute.h>
namespace three {
	class GLRenderer;
	class Scene;
	class Group;
	class Camera;

	namespace gl {
		class GLRenderTarget;
	}

	namespace object3d {
		static size_t _objectIdCount = 0;
		static math::Vector3 DefaultUp = math::Vector3(0,1,0);
		static bool DefaultMatrixAutoUpdate = true;
	}
	using namespace object3d;

	/**
	* Base class for scene graph objects
	*/
	class Object3D {
	public:
		using ptr = std::shared_ptr<Object3D>;
		
		/**
		 * Unique number of this object instance.
		 */
		const size_t id;

		/**
		 *
		 */
		sole::uuid uuid = sole::uuid1();

		/**
		 * Optional name of the object (doesn't need to be unique).
		 */
		std::string name="";

		std::string type="Object3D";

		/**
		 * Object's parent in the scene graph.
		 */
		Object3D* parent = nullptr;

		/**
		 * Array with object's children.
		 */
		std::vector<Object3D::ptr> children;

		/**
		 * Up direction.
		 */
		Vector3 up = Vector3(0,1,0);

		/**
		 * Object's local position.
		 */
		Vector3 position;

		/**
		 * Object's local rotation (Euler angles), in radians.
		 */
		Euler rotation;

		/**
		 * Global rotation.
		 */
		Quaternion quaternion;

		/**
		 * Object's local scale.
		 */
		Vector3 scale = Vector3(1,1,1);

		Matrix4 modelViewMatrix;

		Matrix3 normalMatrix;

		/**
		 * Local transform.
		 */
		Matrix4 matrix;

		/**
		 * The global transform of the object. If the Object3d has no parent, then it's identical to the local transform.
		 */
		Matrix4 matrixWorld;

		/**
		 * When this is set, it calculates the matrix of position, (rotation or quaternion) and scale every frame and also recalculates the matrixWorld property.
		 */
		bool matrixAutoUpdate = DefaultMatrixAutoUpdate;

		/**
		 * When this is set, it calculates the matrixWorld in that frame and resets this property to false.
		 */
		bool matrixWorldNeedsUpdate = true;

		Layers layers;
		/**
		 * Object gets rendered if true.
		 */
		bool visible = true;

		/**
		 * Gets rendered into shadow map.
		 */
		bool castShadow = false;

		/**
		 * Material gets baked in shadow receiving.
		 */
		bool receiveShadow = false;

		/**
		 * When this is set, it checks every frame if the object is in the frustum of the camera. Otherwise the object gets drawn every frame even if it isn't visible.
		 */
		bool frustumCulled = true;

		/**
		 * Overrides the default rendering order of scene graph objects, from lowest to highest renderOrder. Opaque and transparent objects remain sorted independently though. When this property is set for an instance of Group, all descendants objects will be sorted and rendered together.
		 */
		unsigned renderOrder = 0;


		
		///**
		// * An object that can be used to store custom data about the Object3d. It should not hold references to functions as these will not be cloned.
		// */


		// /**
		//  * Custom depth material to be used when rendering to the depth map. Can only be used in context of meshes.
		//  * When shadow-casting with a DirectionalLight or SpotLight, if you are (a) modifying vertex positions in
		//  * the vertex shader, (b) using a displacement map, (c) using an alpha map with alphaTest, or (d) using a
		//  * transparent texture with alphaTest, you must specify a customDepthMaterial for proper shadows.
		//  */
		Material::ptr  customDepthMaterial;

		//  /**
		//   * Same as customDepthMaterial, but used with PointLight.
		//   */
		Material::ptr customDistanceMaterial;

		//   /**
		//	* Used to check whether this or derived classes are Object3Ds. Default is true.
		//	* You should not change this, as it is used internally for optimisation.
		//	*/
		//	readonly isObject3D : true;

		/**
		 * Calls before rendering object
		 */
		//function<void(GLRenderer& renderer, Scene& scene, Camera& camera, std::any& object, Group* group)> onBeforeRender;
		Signal<void(GLRenderer& renderer, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Object3D>& object,const std::shared_ptr<BufferGeometry>& geometry,const std::shared_ptr<Material>& material,const std::shared_ptr<GLRenderTarget>& renderTarget,DrawRange* group)> onBeforeRender;


		/**
		 * Calls after rendering object
		 */
		//function<void(GLRenderer& renderer, Scene& scene, Camera& camera, std::any& object, Group* group)> onAfterRender;
		Signal<void(GLRenderer& renderer, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Object3D>& object,const std::shared_ptr<BufferGeometry>& geometry,const std::shared_ptr<Material>& material,const std::shared_ptr<GLRenderTarget>& renderTarget, DrawRange* group)> onAfterRender;


		std::vector<float> morphTargetInfluences;

		std::unordered_map<std::string, unsigned> morphTargetDictionary;

		

		Geometry::ptr geometry;

		Material::ptr material;

		std::vector<Material::ptr> materials;

		BufferAttribute<float>::ptr instanceColor;

	protected:

		

		void onRotationChange(const math::Euler& rotation);
		
		void onQuaternionChange(const math::Quaternion& quaternion);



	public:
		Object3D();

		Object3D(const Geometry::ptr& geometry, const Material::ptr& material);

		Object3D(const Geometry::ptr& geometry, const std::initializer_list<Material::ptr>& materials);

		Object3D(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials);

		Object3D(const Object3D& object);

		Object3D& operator = (const Object3D& source) {
			return this->copy(source);
		}
		virtual ~Object3D() = default;
		

		static ptr create() {
			return std::make_shared<Object3D>();
		}
		static ptr create(const Geometry::ptr& geometry, const Material::ptr& material) {
			return std::make_shared<Object3D>(geometry, material);
		}

		static ptr create(const Geometry::ptr& geometry, const std::initializer_list<Material::ptr>& materials) {
			return std::make_shared<Object3D>(geometry, materials);
		}
		static ptr create(const Object3D& object) {
			return std::make_shared<Object3D>(object);
		}

		/**
		 * This updates the position, rotation and scale with the matrix.
		 */
		void applyMatrix4(math::Matrix4& matrix);

		Object3D& applyQuaternion(math::Quaternion& quaternion);

		/**
		 *
		 */
		void setRotationFromAxisAngle(math::Vector3& axis, float angle);

		/**
		 *
		 */
		void setRotationFromEuler(math::Euler& euler);

		/**
		 *
		 */
		void setRotationFromMatrix(math::Matrix4& m);

		/**
		 *
		 */
		void setRotationFromQuaternion(math::Quaternion& q);

		/**
		 * Rotate an object along an axis in object space. The axis is assumed to be normalized.
		 * @param axis	A normalized vector in object space.
		 * @param angle	The angle in radians.
		 */
		Object3D& rotateOnAxis(const math::Vector3& axis, float angle);

		/**
		 * Rotate an object along an axis in world space. The axis is assumed to be normalized. Method Assumes no rotated parent.
		 * @param axis	A normalized vector in object space.
		 * @param angle	The angle in radians.
		 */
		Object3D& rotateOnWorldAxis(const math::Vector3& axis, float angle);

		/**
		 *
		 * @param angle
		 */
		Object3D& rotateX(float angle);

		/**
		 *
		 * @param angle
		 */
		Object3D& rotateY(float angle);

		/**
		 *
		 * @param angle
		 */
		Object3D& rotateZ(float angle);

		/**
		 * @param axis	A normalized vector in object space.
		 * @param distance	The distance to translate.
		 */
		Object3D& translateOnAxis(const math::Vector3& axis, float distance);

		/**
		 * Translates object along x axis by distance.
		 * @param distance Distance.
		 */
		Object3D& translateX(float distance);

		/**
		 * Translates object along y axis by distance.
		 * @param distance Distance.
		 */
		Object3D& translateY(float distance);

		/**
		 * Translates object along z axis by distance.
		 * @param distance Distance.
		 */
		Object3D& translateZ(float distance);

		/**
		 * Updates the vector from local space to world space.
		 * @param vector A local vector.
		 */
		Vector3 localToWorld(math::Vector3& vector);

		/**
		 * Updates the vector from world space to local space.
		 * @param vector A world vector.
		 */
		Vector3 worldToLocal(math::Vector3& vector);

		/**
		 * Rotates object to face point in space.
		 * @param vector A world vector to look at.
		 */
		void lookAt(const math::Vector3& vector);

		void lookAt(float x, float y, float z);

		/**
		 * Adds object as child of this object.
		 */
		Object3D& add(const Object3D::ptr & object);

		/**
		 * Removes object as child of this object.
		 */
		Object3D& remove(const Object3D::ptr & object);

		/**
		* Removes all children objects
		*/
		Object3D& removeAll();
		/**
		 * Adds object as a child of this, while maintaining the object's world transform.
		 */
		Object3D& attach(const Object3D::ptr & object);

		/**
		 * Searches through the object's children and returns the first with a matching id.
		 * @param id	Unique number of the object instance
		 */
		//Object3D& getObjectById(unsigned id);

		/**
		 * Searches through the object's children and returns the first with a matching name.
		 * @param name	String to match to the children's Object3d.name property.
		 */
		//Object3D& getObjectByName(std::string name);

		//Object3D& getObjectByProperty(std::string name, std::string);

		math::Vector3& getWorldPosition(math::Vector3& target);

		math::Quaternion& getWorldQuaternion(math::Quaternion& target);


		math::Vector3& getWorldScale(math::Vector3&& target);

		virtual math::Vector3& getWorldDirection(math::Vector3& target);

		//Object3D& raycast(raycaster: Raycaster, intersects : Intersection[]) ;

		void traverse(std::function<void(Object3D&)> callback);

		void traverseVisible(std::function<void(Object3D&)> callback);

		void traverseAncestors(std::function<void(Object3D&)> callback);

		/**
		 * Updates local transform.
		 */
		void updateMatrix();

		/**
		 * Updates global transform of the object and its children.
		 */
		virtual void updateMatrixWorld(bool force = false);

		virtual void updateWorldMatrix(bool updateParents = false, bool updateChildren = false);

		/*toJSON(meta ? : {
		geometries: any;
		materials: any;
		textures: any;
		images: any;
		}) : any;*/

		virtual Object3D* clone(bool recursive = true);

		/**
		 *
		 * @param object
		 * @param recursive
		 */
		virtual Object3D& copy(const Object3D& source,bool recursive = true);

		bool isCamera = false;

		bool isLight = false;

		bool isMesh = false;

		bool isPoints = false;

		bool isLine = false;

		bool isLightProbe = false;

		bool isInstancedMesh = false;

	};
}
#endif