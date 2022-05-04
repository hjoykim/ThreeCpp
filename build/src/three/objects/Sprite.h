#ifndef SPRITE_THREE_H
#define SPRITE_THREE_H
#include <three/core/Object3D.h>
namespace three {
	
	class Sprite : public Object3D {
	public:
		Vector2 center;
		using ptr = std::shared_ptr<Sprite>;
		Sprite(){}
	};
}
#endif
