#ifndef GLRENDERLISTS_THREE_H
#define GLRENDERLISTS_THREE_H
#include <three/core/Object3D.h>
#include <three/objects/Group.h>
#include <three/core/BufferGeometry.h>
#include <three/materials/Material.h>
#include <three/renderers/gl/GLProgram.h>
#include <functional>
namespace three {
	class Scene;
	class Camera;
	namespace gl {
		struct RenderItem {
			using ptr = std::shared_ptr<RenderItem>;

			unsigned id;

			Object3D::ptr object;

			BufferGeometry::ptr geometry;

			Material::ptr material;

			GLProgram::ptr program;

			int groupOrder;

			int renderOrder;

			float z;

			DrawRange* group;

			RenderItem(unsigned id,const Object3D::ptr& object, const BufferGeometry::ptr& geometry, const Material::ptr& material, const GLProgram::ptr& program, DrawRange* group, float z, int groupOrder = 0, int renderOrder = 0)
				: id(id),object(object), geometry(geometry), material(material), program(program), group(group), z(z), groupOrder(groupOrder), renderOrder(renderOrder) {}
		};

		class GLRenderList {
		private:
			const RenderItem::ptr& getNextRenderItem(const Object3D::ptr& object, const BufferGeometry::ptr& geometry, const Material::ptr& material, int groupOrder, float z, DrawRange* group);
		public:
			using ptr = std::shared_ptr<GLRenderList>;

			int renderItemsIndex = 0;

			std::vector<RenderItem::ptr> opaque;

			std::vector<RenderItem::ptr> transparent;

			std::vector<RenderItem::ptr> renderItems;

			GLProgram::ptr defaultProgram = nullptr;

			GLRenderList() {}

			std::function<bool(RenderItem::ptr, RenderItem::ptr)> customOpaqueSort;

			std::function<bool(RenderItem::ptr, RenderItem::ptr) > customTransparentSort;

			virtual ~GLRenderList() = default;				

			void push(const Object3D::ptr& object, const BufferGeometry::ptr& geometry, const Material::ptr& material, int groupOrder, float z, DrawRange* group);

			void unshift(const Object3D::ptr& object, const BufferGeometry::ptr& geometry, const Material::ptr& material, int groupOrder, float z, DrawRange* group);

			void init();

			void sort(std::function<bool(const RenderItem::ptr&, const RenderItem::ptr&)> customOpaqueSort=nullptr, std::function<bool(const RenderItem::ptr&, const RenderItem::ptr&)> customTransparentSort=nullptr);

			void finish();
		};
		inline bool painterSortStable(const RenderItem::ptr& a, const RenderItem::ptr& b) {
			if (a->groupOrder != b->groupOrder)
			{
				return b->groupOrder > a->groupOrder;
			}
			else if (a->renderOrder != b->renderOrder)
			{
				return b->renderOrder > a->renderOrder;
			}
			else if (a->program!=nullptr && b->program!=nullptr &&( a->program->id != b->program->id))
			{
				return b->program->id > a->program->id;
			}
			else if (a->material->id != b->material->id)
			{
				return b->material->id > a->material->id;
			}
			else if (a->z != b->z)
			{
				return b->z > a->z;
			}
			else
			{
				return b->id > a->id;
			}
		}
		inline bool reversePainterSortStable(const RenderItem::ptr& a, const RenderItem::ptr& b) {
			if (a->groupOrder != b->groupOrder)
			{
				return b->groupOrder > a->groupOrder;
			}
			else if (a->renderOrder != b->renderOrder)
			{
				return b->renderOrder > a->renderOrder;
			}
			else if (a->program != nullptr && b->program != nullptr && (a->program->id != b->program->id))
			{
				return b->program->id > a->program->id;
			}
			else if (a->material->id != b->material->id)
			{
				return b->material->id > a->material->id;
			}
			else if (a->z != b->z)
			{
				return b->z > a->z;
			}
			else
			{
				return b->id > a->id;
			}
		}
		using glRenderListMapPtr = std::shared_ptr<std::unordered_map<sole::uuid, GLRenderList::ptr>>;

		class GLRenderLists : public std::unordered_map<sole::uuid, glRenderListMapPtr> {
		/*private:
			using ScenePtr = std::shared_ptr<Scene>;
			using CameraPtr = std::shared_Ptr<Camera>;*/
		public:
			using ScenePtr = std::shared_ptr<Scene>;
			using CameraPtr = std::shared_ptr<Camera>;

			using ptr = std::shared_ptr<GLRenderLists>;

			GLRenderLists() :std::unordered_map<sole::uuid, glRenderListMapPtr>() {}
			virtual ~GLRenderLists() = default;

			void onSceneDispose(const Scene& scene);

			const GLRenderList::ptr& get(ScenePtr scene,CameraPtr camera);
		};
	}
}
#endif