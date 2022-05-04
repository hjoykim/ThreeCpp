#ifndef SKINNED_MESH_THREE_H
#define SKINNED_MESH_THREE_H
#include <three/objects/Mesh.h>
#include <string>
#include <three/math/Matrix4.h>
#include <three/objects/Skeleton.h>
using namespace three::math;
namespace three {
	class SkinnedMesh : public Mesh {
	public :

		std::string bindMode;

		Matrix4 bindMatrix;

		Matrix4 bindMatrixInverse;

		Skeleton::ptr skeleton = nullptr;

		using ptr = std::shared_ptr<SkinnedMesh>;

		SkinnedMesh();

		SkinnedMesh(const Geometry::ptr& geometry, const Material::ptr& material);

		SkinnedMesh(const Geometry::ptr& geometry, const std::initializer_list<Material::ptr>& materials);

		SkinnedMesh(const SkinnedMesh& mesh);

	};
}
#endif