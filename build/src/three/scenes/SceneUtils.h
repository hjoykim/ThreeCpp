#ifndef SCENEUTILS_H
#define SCENEUTILS_H
#include <three/objects/Group.h>
namespace three {
	class SceneUtils {
	public :
		SceneUtils() {}
		static std::shared_ptr<Group> createMultiMaterialObject(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials)
        {
            auto group = std::make_shared<Group>();

            for (int i = 0;i < materials.size();i++) {
                auto mesh = std::make_shared<Mesh>(geometry, materials[i]);

                group->add(std::dynamic_pointer_cast<Object3D>(mesh));
            }
            return group;
        }
	};
}
#endif
