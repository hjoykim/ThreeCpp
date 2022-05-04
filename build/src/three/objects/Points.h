#ifndef POINTS_THREE_H
#define POINTS_THREE_H
#include <three/core/Object3D.h>
namespace three {
	class Points :public Object3D{

	public:
		using ptr = std::shared_ptr<Points>;
		Points() :Object3D() 
		{
			this->type = "Points";
			this->isPoints = true;
		}
		Points(const Geometry::ptr& geometry, const std::initializer_list<Material::ptr>& materials) : Object3D(geometry, materials) {}

		Points(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials) : Object3D(geometry, materials) {}

		Points(const Geometry::ptr& geometry, const Material::ptr& material) : Object3D(geometry, material)
		{
			this->type = "Points";
			this->isPoints = true;
		}
		static ptr create() {
			return std::make_shared<Points>();
		}
		static ptr create(const Geometry::ptr& geometry, const Material::ptr& material) 
		{
			return std::make_shared<Points>(geometry, material);
		}
		static ptr create(const Geometry::ptr& geometry, const std::vector<Material::ptr>& material)
		{
			return std::make_shared<Points>(geometry, material);
		}
		static ptr create(const Geometry::ptr& geometry, const std::initializer_list<Material::ptr>& material)
		{
			return std::make_shared<Points>(geometry, material);
		}
	};
}
#endif