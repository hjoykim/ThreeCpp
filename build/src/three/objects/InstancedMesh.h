#ifndef INSTANCEDMESH_THREE_H
#define INSTANCEDMESH_THREE_H
#include <three/objects/Mesh.h>
#include <three/core/BufferAttribute.h>
#include <three/core/Geometry.h>
#include <three/materials/Material.h>
#include <memory>
namespace three {
	class InstancedMesh : public Mesh {
	public:
		BufferAttribute<float> instanceMatrix;

		int count = 0;

		using ptr = std::shared_ptr<InstancedMesh>;		

		InstancedMesh() : Mesh() {
			isInstancedMesh = true;
		}

		InstancedMesh(const Geometry::ptr& geometry, const Material::ptr& material, int count) : Mesh(geometry, material), count(count) {
			isInstancedMesh = true;
		}

		InstancedMesh(const Geometry::ptr& geometry, const std::initializer_list<Material::ptr>& materials, int count) :Mesh(geometry, materials), count(count) {
			isInstancedMesh = true;
		}

		InstancedMesh(const InstancedMesh& mesh) : Mesh(mesh) {
			isInstancedMesh = true;
		}
	};
}
#endif