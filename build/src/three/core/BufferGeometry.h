#ifndef BUFFER_GEOMETRY_THREE_H
#define BUFFER_GEOMETRY_THREE_H
#include <three/utils/osdecl.h>
#include <vector>
#include <memory>
#include <limits>
#include <unordered_map>
#include <three/core/Geometry.h>
#include <three/core/BufferAttribute.h>
#include <three/Types.h>
#include <three/core/Attribute.h>

//namespace three {


//	enum class IndexAttributeName : size_t {
//		morphTarget, morphNormal
//	};
//	using IndexedAttributeKey = std::pair<IndexAttributeName, size_t>;
//}
//namespace std
//{
//	template<> struct hash<three::IndexedAttributeKey>
//	{
//		typedef three::IndexedAttributeKey argument_type;
//		typedef std::size_t result_type;
//		result_type operator()(argument_type const& s) const noexcept
//		{
//			auto h = three::EnumHash{}(s.first);
//			three::hash_combine(h, s.second);
//			return h;
//		}
//	};
//}

namespace three {
	using namespace math;
	class Object3D;
	class DirectGeometry;

	class BufferGeometry : public Geometry {
	public:
		BufferGeometry();

		BufferGeometry(const BufferGeometry& source);

		BufferGeometry(const BufferAttribute<float>::ptr& position, const BufferAttribute<float>::ptr& color);

		using ptr = std::shared_ptr<BufferGeometry>;

		static ptr create() {
			return std::make_shared<BufferGeometry>();
		}		

		static ptr create(const BufferAttribute<float>::ptr& position, const BufferAttribute<float>::ptr& color) {
			return std::make_shared<BufferGeometry>(position, color);
		}

		BufferAttribute<unsigned>::ptr index;

		/*BufferAttribute<float>::ptr position;

		BufferAttribute<float>::ptr normal;

		BufferAttribute<float>::ptr color;

		BufferAttribute<float>::ptr uv;

		BufferAttribute<float>::ptr uv2;*/



		/*BufferAttribute<float>::ptr tangent;

		BufferAttribute<float>::ptr bitangent;

		BufferAttribute<float>::ptr lineDistance;*/

		std::vector<DrawRange> drawCalls;

		std::vector<DrawRange> offsets;

		/*BufferAttribute<float>::ptr skinIndex;

		BufferAttribute<float>::ptr skinWeight;*/

		std::unordered_map<std::string, std::vector<std::shared_ptr<BufferAttribute<float>>>> morphAttributes;

		//for only morph attribute + i
		std::unordered_map<AttributeNameKey,std::shared_ptr<BufferAttribute<float>>> attributes;

		//std::unordered_map<IndexedAttributeKey, BufferAttribute<float>::ptr> indexedAttribute;

		bool morphTargetsRelative = false;

		DrawRange drawRange;// = DrawRange() { start = 0, materialIndex = -1, count = std::numeric_limits<int>::infinity() }

		std::vector<DrawRange> groups;		

		const BufferAttribute<unsigned>::ptr& getIndex();

		BufferGeometry& setIndex(const BufferAttribute<unsigned>::ptr& indices);

		/*BufferGeometry& setPosition(const BufferAttribute<float>::ptr& position);

		BufferGeometry& setNormal(const BufferAttribute<float>::ptr& normal);

		BufferGeometry& setColor(const BufferAttribute<float>::ptr& color);

		BufferGeometry& setUV(const BufferAttribute<float>::ptr& uv);

		BufferGeometry& setUV2(const BufferAttribute<float>::ptr& uv2);

		BufferGeometry& setTangent(const BufferAttribute<float>::ptr& tangent);

		BufferGeometry& setBitangent(const BufferAttribute<float>::ptr& bitangent);

		BufferGeometry& setLineDistance(const BufferAttribute<float>::ptr& lineDistance);

		BufferGeometry& setSkinIndexAttribute(const BufferAttribute<float>::ptr& skinIndexAttribute);

		BufferGeometry& setSkinWeightAttribute(const BufferAttribute<float>::ptr skinWeightAttribute);*/

		BufferAttribute<float>::ptr& getAttribute(AttributeName name);

		BufferAttribute<float>::ptr& getAttribute(const std::string& name);

		BufferAttribute<float>::ptr& getAttribute(AttributeName name,size_t index) {
			return attributes[{name, index}];
		}
		void setAttribute(AttributeName name, const BufferAttribute<float>::ptr& attribute) {
			attributes[{name, 0}] = attribute;
		}
		void setAttribute(AttributeName name, size_t index, const BufferAttribute<float>::ptr attribute) {
			attributes[{name, index}]= attribute;
		}
		void deleteAttribute(AttributeName name,size_t index) {
			attributes.erase({ name,index });
		}
		//bool containsAttribute(const std::string &name) {
		//	size_t key = getStringHashcode(name);
		//	if (attributes.count(key) > 0) return true;
		//	return false;
		//}
		//template<typename T>
		//const std::shared_ptr<BufferAttribute<T>>& getAttribute(const std::string& name) {
		//	
		//	size_t key = getStringHashcode(name);
		//	if (attributes.count(key) > 0)
		//		return attributes[key];
		//	else 
		//	{
		//		std::shared_ptr<BufferAttribute<T>> nullAttribute = nullptr;
		//		return nullAttribute;
		//	}
		//}

		//template<typename T>
		//void setAttribute(const std::string& name, const std::shared_ptr<BufferAttribute<T>>& attribute) {
		//	size_t key = getStringHashcode(name);
		//	attributes.insert_or_assign(key, attribute);
		//	/*if (attributes.count(key) > 0) {
		//		std::shared_ptr<BufferAttribute<T>> temp = std::any_cast<std::shared_ptr<BufferAttribute<T>>>(attributes[key]);
		//		delete temp.get();
		//		temp.reset();
		//	}

		//	attributes[key] = attribute;*/
		//}

		void deleteAttribute(AttributeName name);

		virtual void addGroup(const unsigned start, const unsigned count, const int materialIndex);

		void clearGroups();

		void setDrawRange(const unsigned start, const unsigned count);



		virtual BufferGeometry& applyMatrix4(const Matrix4& matrix);

		virtual BufferGeometry& rotateX(const float angle);

		virtual BufferGeometry& rotateY(const float angle);

		virtual BufferGeometry& rotateZ(const float angle);

		virtual BufferGeometry& translate(const float x, const float y, const float z);

		virtual BufferGeometry& scale(const float x, const float y, const float z);

		virtual BufferGeometry& lookAt(const Vector3& vector);

		virtual BufferGeometry& center();

		virtual BufferGeometry& setFromObject(const Object3D& object);

		virtual BufferGeometry& setFromPoints(const std::vector<Vector3>& points);

		virtual BufferGeometry& updateFromObject(const Object3D& object);

		virtual BufferGeometry& fromGeometry(Geometry& geometry);

		virtual BufferGeometry& fromDirectGeometry(DirectGeometry& geometry);

		virtual void computeBoundingBox() override;

		virtual void computeBoundingSphere() override;

		virtual void computeVertexNormals(bool areaWeighted = false) override;

		virtual BufferGeometry& merge(BufferGeometry& geometry, int offset);

		virtual void normalizeNormals();

		virtual void computeFaceNormals() override;

		virtual BufferGeometry* clone() override ;

		virtual BufferGeometry& copy(const BufferGeometry& source);

		virtual void toNonIndexed(const BufferGeometry::ptr& geometry2);

		
		virtual const BufferAttribute<unsigned>::ptr& mergeAttributeInt(const BufferAttribute<unsigned>::ptr& source, const BufferAttribute<unsigned>::ptr& target, int offset);

		virtual const BufferAttribute<float>::ptr& mergeAttribute(const BufferAttribute<float>::ptr& source, const BufferAttribute<float>::ptr& target, int offset);

		
	};

}
#endif