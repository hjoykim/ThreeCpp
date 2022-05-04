#include "GLRenderLists.h"
#include <three/scenes/Scene.h>
#include <three/cameras/Camera.h>
namespace three {

	namespace gl {
		const RenderItem::ptr& GLRenderList::getNextRenderItem(const Object3D::ptr& object, const BufferGeometry::ptr& geometry, const Material::ptr& material, int groupOrder, float z, DrawRange* group)
		{
			int i = renderItemsIndex;
			if ((renderItems.size()==0) ||(renderItemsIndex > (renderItems.size()-1))) {				
				
				auto renderItem = std::make_shared<RenderItem>(object->id, object, geometry, material, nullptr, group, z, groupOrder, object->renderOrder);

				renderItems.push_back(renderItem);

				renderItemsIndex++;

				return renderItems[i];
			}
			else {
				
				renderItems[i]->id = object->id;
				renderItems[i]->object = object;
				renderItems[i]->geometry = geometry;
				renderItems[i]->material = material;
				renderItems[i]->program = nullptr;
				renderItems[i]->group = group;
				renderItems[i]->z = z;
				renderItems[i]->groupOrder = groupOrder;
				renderItems[i]->renderOrder = object->renderOrder;

				renderItemsIndex++;

				return renderItems[i];
			}	
		}
		
		void GLRenderList::push(const Object3D::ptr& object, const BufferGeometry::ptr& geometry, const Material::ptr& material, int groupOrder, float z, DrawRange* group)
		{
			auto renderItem = getNextRenderItem(object, geometry, material, groupOrder, z, group);

			if (material->transparent)
				transparent.push_back(renderItem);
			else
				opaque.push_back(renderItem);
		}
		void GLRenderList::unshift(const Object3D::ptr& object, const BufferGeometry::ptr& geometry, const Material::ptr& material, int groupOrder, float z, DrawRange* group)
		{
			auto renderItem = getNextRenderItem(object, geometry, material, groupOrder, z, group);

			if (material->transparent) {				
				transparent.insert(transparent.begin(),1,renderItem);
			}
			else {
				opaque.insert(opaque.begin(), 1, renderItem);				
			}
		}
		void GLRenderList::init()
		{
			opaque.clear();
			transparent.clear();
			renderItems.clear();
			renderItemsIndex = 0;
		}
		void GLRenderList::sort(std::function<bool(const RenderItem::ptr&, const RenderItem::ptr&)> customOpaqueSort, std::function<bool(const RenderItem::ptr&, const RenderItem::ptr&)> customTransparentSort)
		{
			if (opaque.size() > 0) std::sort(opaque.begin(), opaque.end(), customOpaqueSort==nullptr?painterSortStable:customOpaqueSort);
			if (transparent.size() > 0) std::sort(transparent.begin(), transparent.end(), customTransparentSort == nullptr ? reversePainterSortStable : customTransparentSort);
		}
		void GLRenderList::finish()
		{
			/*for (unsigned i = 0; i < renderItemsIndex; i++) {
				if (renderItems[i]->id == std::numeric_limits<unsigned>::quiet_NaN()) break;
				renderItems[i]->id = std::numeric_limits<unsigned>::quiet_NaN();
				renderItems[i]->object.reset();
				renderItems[i]->object = nullptr;
				renderItems[i]->material.reset();
				renderItems[i]->material = nullptr;
				renderItems[i]->program.reset();
				renderItems[i]->program = nullptr;
				renderItems[i].reset();
			}*/
			renderItems.clear();
			renderItemsIndex = 0;
		}
		void GLRenderLists::onSceneDispose(const Scene& scene)
		{
			if (count(scene.uuid) > 0) {
				std::shared_ptr<std::unordered_map<sole::uuid, GLRenderList::ptr>> cameraMapPtr = this->at(scene.uuid);
				cameraMapPtr->clear();
				this->erase(scene.uuid);
			}

		}
		const GLRenderList::ptr& GLRenderLists::get(ScenePtr scene, CameraPtr camera)
		{
			GLRenderList::ptr list = nullptr;
			std::shared_ptr<std::unordered_map<sole::uuid, GLRenderList::ptr>> cameras;
			if (count(scene->uuid) == 0) {
				list = std::make_shared<GLRenderList>();
				cameras = std::make_shared<std::unordered_map<sole::uuid, GLRenderList::ptr>>();
				cameras->insert({ camera->uuid,list });

				this->insert({ scene->uuid,cameras });
				
				scene->onDisposed.connect(*this, &GLRenderLists::onSceneDispose);
			}
			else {
				cameras = this->at(scene->uuid);
				if (cameras->count(camera->uuid) == 0) {
					list = std::make_shared<GLRenderList>();
					cameras->insert({ camera->uuid,list });
				}
				/*else {
					list = cameras->at(camera->uuid.str());
				}*/
			}
			return this->at(scene->uuid)->at(camera->uuid);
		}
	}
}
