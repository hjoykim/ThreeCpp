#ifndef MESH_THREE_H
#define MESH_THREE_H
#include <memory>
#include <vector>
#include <three/core/Object3D.h>
#include <three/core/Geometry.h>
#include <three/materials/Material.h>
namespace three {
	
	class Mesh : public Object3D {	
	public:

		Mesh();

		Mesh(const Geometry::ptr& geometry, const Material::ptr& material=nullptr);

		Mesh(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials);

		Mesh(const Geometry::ptr& geometry, const std::initializer_list<Material::ptr>& materials);

		Mesh(const Mesh& mesh);
		

		using ptr = std::shared_ptr<Mesh>;		
		unsigned drawMode;

		

		static ptr create() {
			return std::make_shared<Mesh>(Mesh());
		}
		static ptr create(const Geometry::ptr& geometry, const Material::ptr& material=nullptr) {
			return std::make_shared<Mesh>(geometry, material);
		}

		static ptr create(const Geometry::ptr& geometry, const std::initializer_list<Material::ptr>& materials) {
			return std::make_shared<Mesh>(geometry, materials);
		}
		static ptr create(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials) {
			return std::make_shared<Mesh>(geometry, materials);
		}
		void set(const Geometry::ptr& geometry, const Material::ptr& material);
		void updateMorphTargets();
		//void raycast();

		virtual Mesh* clone(bool recursive = true);

		/**
		 *
		 * @param object
		 * @param recursive
		 */
		virtual Mesh& copy(const Mesh& source, bool recursive = true);
	};
}
#endif