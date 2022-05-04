#ifndef BONE_THREE_H
#define BONE_THREE_H
#include <three/core/Object3D.h>
namespace three {
	class Bone : public Object3D {
	public :
		using ptr = std::shared_ptr<Bone>;

		Bone() : Object3D() 
		{
			type = "Bone";
		}
		Bone(const Bone& source) : Object3D(source) {

		}
		static ptr create() {
			return std::make_shared <Bone>();
		}
	};
}
#endif