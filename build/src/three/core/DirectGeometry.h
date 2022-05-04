#ifndef DIRECT_GEOMETRY_THREE_H
#define DIRECT_GEOMETRY_THREE_H
#include <memory>
#include <vector>
#include <string>
#include <three/core/Geometry.h>
#include <three/Types.h>
#include <three/core/BufferAttribute.h>
using namespace three::math;
namespace three {
	class DirectGeometry : public Geometry {	
	public:

		using ptr = std::shared_ptr<DirectGeometry>;

		std::vector<unsigned> indices;

		std::vector<DrawRange> groups;
		
		//ThreeDataTable morphPositionNormal;
		std::unordered_map<std::string, std::vector<MorphTarget>> morphPositionNormal;

		DirectGeometry() :Geometry() {}

		using ptr = std::shared_ptr<DirectGeometry>;

		void computeGroups(const Geometry& geometry);

		DirectGeometry& fromGeometry(Geometry& geometry);

	};
}
#endif