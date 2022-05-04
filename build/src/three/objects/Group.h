#ifndef GROUP_THREE_H
#define GROUP_THREE_H
#include <memory>
#include <three/core/Object3D.h>
namespace three {
	class Group : public Object3D {
	public:
		std::vector<std::string> materialLibraries;
		using ptr = std::shared_ptr<Group>;

		Group() : Object3D() 
		{
			this->type = "Group";
		}
		
		static ptr create() {
			return std::make_shared<Group>();
		}
	};
}
#endif