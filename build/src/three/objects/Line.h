#ifndef LINE_THREE_H
#define LINE_THREE_H
#include <memory>
#include <three/utils/osdecl.h>
#include <three/core/Object3D.h>
#include <three/core/Geometry.h>
#include <three/core/BufferGeometry.h>
#include <three/materials/LineBasicMaterial.h>
#include <three/Constants.h>
namespace three {
	class Line :public Object3D {
	public:
		using ptr = std::shared_ptr<Line>;		
		
		unsigned lineStrip = 0;

		unsigned linePieces = 1;


        Line(const Geometry::ptr& geometry = nullptr, const Material::ptr& material = nullptr);

        Line(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials);
     

		static ptr create(const Geometry::ptr& geometry=nullptr, const Material::ptr& material=nullptr) {
			return std::make_shared<Line>(geometry, material);
		}

		static ptr create(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials) {
			return std::make_shared<Line>(geometry, materials);
		}

        virtual Line& computeLineDistances(); 
 

		virtual void raycast();

	};

	class LineLoop : public Line {
	public:
		using ptr = std::shared_ptr<LineLoop>;
		LineLoop(const Geometry::ptr& geometry = nullptr, const Material::ptr& material = nullptr) : Line(geometry, material) {
			type = "LineLoop";
		}
		LineLoop(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials) : Line(geometry, materials) {
			type = "LineLoop";
		}

	};

	class LineSegments : public Line {
	public:
		using ptr = std::shared_ptr<LineSegments>;


		LineSegments(const Geometry::ptr& geometry = nullptr, const Material::ptr& material = nullptr) : Line(geometry, material) {
			type = "LineSegments";
		}


		LineSegments(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials) : Line(geometry, materials) {
			type = "LineSegments";
		}

		static ptr create(const Geometry::ptr& geometry = nullptr, const Material::ptr& material = nullptr) {
			return std::make_shared<LineSegments>(geometry, material);
		}

		static ptr create(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials) {
			return std::make_shared<LineSegments>(geometry, materials);
		}

		virtual LineSegments& computeLineDistances();
	};
}
#endif