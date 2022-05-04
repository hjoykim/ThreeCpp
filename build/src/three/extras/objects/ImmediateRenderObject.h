#ifndef IMMEDIATERENDEROBJECT_THREE_H
#define IMMEDIATERENDEROBJECT_THREE_H
#include <three/core/Object3D.h>
namespace three {
	namespace extras {
		class ImmediateRenderObject : public Object3D {
		public:
			ImmediateRenderObject() : Object3D() {}
			ImmediateRenderObject(const ImmediateRenderObject& source) : Object3D(source) {}
			void render() {}
		};
	}
}
#endif