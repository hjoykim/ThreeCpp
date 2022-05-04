#ifndef GEOMETRY_THREE_H
#define GEOMETRY_THREE_H
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <three/math/Color.h>
#include <three/math/Box3.h>
#include <three/math/Sphere.h>
#include <three/core/Face3.h>
#include <three/utils/sole.h>
#include <three/math/Vector4.h>
#include <three/math/Vector2.h>
#include <three/math/Vector3.h>
#include <three/math/Matrix3.h>
#include <three/math/Matrix4.h>
#include <three/utils/simplesignal.h>
#include <three/Types.h>
using namespace three::math;
namespace three {

	class Mesh;

	class BufferGeometry;

	class DirectGeometry;

	using bufferPtr = std::shared_ptr<BufferGeometry>;
	using directPtr = std::shared_ptr<DirectGeometry>;

	enum class AttributeName : size_t {
		index, color, position, normal, uv, uv2, lineDistance, tangent, bitangent, skinIndex, skinWeight, 
		morphTarget,
		morphNormal,
		instanceMatrix,
		unknown

	};
	using AttributeNameKey = std::pair<AttributeName, size_t>;
	/*struct AttributeNameKey_hash {
		template<class T1,class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	 };*/
}
namespace std {
	template<> struct hash<three::AttributeNameKey> {
		typedef three::AttributeNameKey argument_type;
		
		std::size_t operator()(const argument_type& s) const {
			auto h = three::EnumHash{}(s.first);
			three::hash_combine(h, s.second);
			return h;
		}
	};
}
namespace three {

	struct MorphTarget {
		std::string name;
		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;

	};

	struct MorphColor {
		std::string name;
		std::vector<Color> colors;
	};

	struct MorphNormals {
		std::string name;
		std::vector<Vector3> normals;
		std::vector<Vector3> faceNormals;
		std::vector<std::vector<Vector3>> vertexNormals;
	};
	

	struct MorphNormal {

	};
	namespace geometry {
		static size_t _geometryId = 0;
	}
	using namespace geometry;


	class Geometry {
	private:

		//void addFace(unsigned a, unsigned b, unsigned c, unsigned materialIndex, std::vector<float> normals, std::vector<float> uvs, std::vector<float>uvs2);
		void addFace(unsigned a, unsigned b, unsigned c, unsigned materialIndex, const std::shared_ptr<BufferAttribute<float>>& normals, const std::shared_ptr<BufferAttribute<float>>& uvs, const std::shared_ptr<BufferAttribute<float>>& uvs2);
		
	public:
		using OnDispose = Signal<void(Geometry*)>;

		OnDispose onDispose;

		Geometry();
		
		Geometry(const Geometry& source);

		virtual ~Geometry() {}
		
		std::string type = "Geometry";

		using ptr = std::shared_ptr<Geometry>;

		static ptr create() {
			return std::make_shared<Geometry>();
		}

		static ptr create(const Geometry& source) {
			return std::make_shared<Geometry>(source);
		}		
		/**
		 * Unique number of this geometry instance
		 */
		const size_t id;

		sole::uuid uuid = sole::uuid1();

		/**
		 * Name for this geometry. Default is an empty string.
		 */
		std::string name="";
		/**
		 * The array of vertices hold every position of points of the model.
		 * To signal an update in this array, Geometry.verticesNeedUpdate needs to be set to true.
		 */
		std::vector<Vector3> vertices;
		/**
		 * Array of vertex colors, matching number and order of vertices.
		 * Used in ParticleSystem, Line and Ribbon.
		 * Meshes use per-face-use-of-vertex colors embedded directly in faces.
		 * To signal an update in this array, Geometry.colorsNeedUpdate needs to be set to true.
		 */
		std::vector<Color> colors;

		/**
		 * Array of triangles or/and quads.
		 * The array of faces describe how each vertex in the model is connected with each other.
		 * To signal an update in this array, Geometry.elementsNeedUpdate needs to be set to true.
		 */
		std::vector<Face3> faces;

		std::vector<Vector3> normals;

		std::vector<Vector2> uvs;

		std::vector<Vector2> uvs2;
		/**
		 * Array of face UV layers.
		 * Each UV layer is an array of UV matching order and number of vertices in faces.
		 * To signal an update in this array, Geometry.uvsNeedUpdate needs to be set to true.
		 */
		//std::vector<std::vector<std::vector<Vector2>>> faceVertexUvs;
		std::vector<std::vector<std::vector<Vector2>>> faceVertexUvs;

		/**
		 * Array of morph targets. Each morph target is a Javascript object:
		 *
		 *		 { name: "targetName", vertices: [ new THREE.Vector3(), ... ] }
		 *
		 * Morph vertices match number and order of primary vertices.
		 */
		std::vector<MorphTarget> morphTargets;
		
		//gl::HashValues morphTargets;
		/**
		 * Array of morph normals. Morph normals have similar structure as morph targets, each normal set is a Javascript object:
		 *
		 *		 morphNormal = { name: "NormalName", normals: [ new THREE.Vector3(), ... ] }
		 */
		//std::unordered_map<std::string,std::vector<BufferAttribute<float>::ptr>> morphNormals;
		std::vector<MorphNormals> morphNormals;
		//gl::HashValues morphNormals;
		/**
		 * Array of skinning weights, matching number and order of vertices.
		 */
		std::vector<Vector4> skinWeights;
		/**
		 * Array of skinning indices, matching number and order of vertices.
		 */
		std::vector<Vector4> skinIndices;

		std::vector<float> lineDistances;
		/**
		 * Bounding box.
		 */
		Box3 boundingBox;

		/**
		 * Bounding sphere.
		 */
		Sphere boundingSphere;

		/**
		 * Set to true if the vertices array has been updated.
		 */
		bool verticesNeedUpdate = false;

		/**
		 * Set to true if the faces array has been updated.
		 */
		bool elementsNeedUpdate = false;

		/**
		 * Set to true if the uvs array has been updated.
		 */
		bool uvsNeedUpdate = false;

		/**
		 * Set to true if the normals array has been updated.
		 */
		bool normalsNeedUpdate = false;

		/**
		 * Set to true if the colors array has been updated.
		 */
		bool colorsNeedUpdate = false;

		/**
		 * Set to true if the linedistances array has been updated.
		 */
		bool lineDistancesNeedUpdate = false;

		/**
		 *
		 */
		bool groupsNeedUpdate = false ;

		bool isGeometry = true;

		bufferPtr __bufferGeometry;

		directPtr __directGeometry;

		

		/**
		 * Bakes matrix transform directly into vertex coordinates.
		 */
		virtual Geometry& applyMatrix4(Matrix4& matrix);

		virtual Geometry& rotateX(float angle);

		virtual Geometry& rotateY(float angle);

		virtual Geometry& rotateZ(float angle);

		virtual Geometry& translate(float x,float y,float z);

		virtual Geometry& scale(float x,float y,float z);
		
		virtual Geometry& lookAt(Vector3& vector);

		Geometry& fromBufferGeometry(BufferGeometry& geometry);

		virtual Geometry& center();

		virtual Geometry& normalize();

		/**
		 * Computes face normals.
		 */
		virtual void computeFaceNormals();

		/**
		 * Computes vertex normals by averaging face normals.
		 * Face normals must be existing / computed beforehand.
		 */
		virtual void computeVertexNormals(bool areaWeighted=false);

		/**
		 * Compute vertex normals, but duplicating face normals.
		 */
		virtual void computeFlatVertexNormals();

		/**
		 * Computes morph normals.
		 */
		virtual void computeMorphNormals();

		/**
		 * Computes bounding box of the geometry, updating {@link Geometry.boundingBox} attribute.
		 */
		virtual void computeBoundingBox();

		/**
		 * Computes bounding sphere of the geometry, updating Geometry.boundingSphere attribute.
		 * Neither bounding boxes or bounding spheres are computed by default. They need to be explicitly computed, otherwise they are null.
		 */
		virtual void computeBoundingSphere();

		virtual void merge(Geometry& geometry, Matrix4* matrix = nullptr, unsigned	materialIndexOffset = 0);
		

		virtual void mergeMesh(Mesh& mesh);

		/**
		 * Checks for duplicate vertices using hashmap.
		 * Duplicated vertices are removed and faces' vertices are updated.
		 */
		virtual int mergeVertices();

		Geometry& setFromPoints(Vector2* points, unsigned length);

		Geometry& setFromPoints(Vector3* points, unsigned length); 

		void sortFacesByMaterialIndex();

		

		/**
		 * Creates a new clone of the Geometry.
		 */
		virtual Geometry* clone();

		virtual Geometry& copy(const Geometry& source);

		/**
		 * Removes The object from memory.
		 * Don't forget to call this method when you remove an geometry because it can cuase meomory leaks.
		 */
		virtual void dispose() {
			onDispose.emitSignal(this);
		}

		// These properties do not exist in a normal Geometry class, but if you use the instance that was passed by JSONLoader, it will be added.
		//bones: Bone[];
		//animation: AnimationClip;
		//animations: AnimationClip[];	
		
	};
	inline bool materialIndexSort(const Face3& a, const Face3& b) {
		return a.materialIndex < b.materialIndex;
	}
	
}
#endif