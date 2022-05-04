#ifndef SKELETON_THREE_H
#define SKELETON_THREE_H
#include <three/core/Object3D.h>
#include <three/objects/Bone.h>
#include <three/textures/DataTexture.h>
namespace three {
	class Skeleton : public Object3D {
	public:
		using ptr = std::shared_ptr<Skeleton>;

		bool useVertexTexture = false;

		std::vector<Bone> bones;

		std::vector<unsigned char> boneMatrices;

		DataTexture::ptr boneTexture = nullptr;

		int boneTextureSize = -1;

		std::vector<Matrix4> boneInverses;

		int frame = -1;

		Skeleton() : Object3D() {}

		Skeleton(const std::vector<Bone>& bones, const std::vector<Matrix4>& boneInverses);

		static ptr create() {
			return std::make_shared<Skeleton>();
		}

		static ptr create(const std::vector<Bone>& bones, const std::vector<Matrix4>& boneInverses) {
			return std::make_shared<Skeleton>(bones, boneInverses);
		}

		void calculateInverses();

		void pose();

		void update();

		Bone* getBoneByName(const std::string& name);

		void dispose();

	};
}
#endif