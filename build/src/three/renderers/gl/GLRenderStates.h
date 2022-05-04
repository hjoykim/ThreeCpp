#ifndef GLRENDER_STATES_THREE_H
#define GLRENDER_STATES_THREE_H
#include <three/scenes/Scene.h>
#include <three/lights/Light.h>
#include <three/renderers/gl/GLLights.h>
#include <three/cameras/Camera.h>
namespace three {
	namespace gl {
		
		class GLRenderState
		{
		public:
			struct RenderState {
				std::vector<Light::ptr> lightsArray;
				std::vector<Light::ptr> shadowsArray;
				GLLights lights;
			};

			using ptr = std::shared_ptr<GLRenderState>;

			RenderState state;

			GLRenderState() {}

			using ptr = std::shared_ptr<GLRenderState>;

			static ptr create() {
				return std::make_shared<GLRenderState>();
			}
			virtual ~GLRenderState() = default;

			void init() {
				state.lightsArray.clear();
				state.shadowsArray.clear();
			}

			void pushLight(const Light::ptr& light) {
				state.lightsArray.push_back(light);
			}
			void pushShadow(const Light::ptr& shadowLight) {
				state.shadowsArray.push_back(shadowLight);
			}
			void setupLights(const Camera::ptr& camera) {
				state.lights.setup(state.lightsArray, camera);
			}
		};

		
		class GLRenderStates {
		public:
			using ptr = std::shared_ptr<GLRenderStates>;

			std::unordered_map<sole::uuid,std::unordered_map<sole::uuid,GLRenderState::ptr>> renderStates;
			

			GLRenderStates() {}
			virtual ~GLRenderStates() = default;

			static ptr create() {
				return std::make_shared<GLRenderStates>();
			}
			void onSceneDispose(const Scene& scene) {
				
				if (renderStates.count(scene.uuid) > 0) {
					renderStates[scene.uuid].clear();
					renderStates.erase(scene.uuid);
				}
			}

			const GLRenderState::ptr& get(const Scene::ptr& scene, const Camera::ptr& camera) {
				GLRenderState::ptr renderState = nullptr;
				if (renderStates.count(scene->uuid)<1) {
					renderState = GLRenderState::create();
					renderStates.insert({ scene->uuid, std::unordered_map<sole::uuid,GLRenderState::ptr>()});
					renderStates[scene->uuid].insert({ camera->uuid,renderState });
					scene->onDisposed.connect(*this, &GLRenderStates::onSceneDispose);
					return renderStates[scene->uuid][camera->uuid];
					//return renderState;
				}
				else {
					std::unordered_map<sole::uuid,GLRenderState::ptr> list = renderStates[scene->uuid];
					GLRenderState::ptr tempState = list[camera->uuid];
					int count = list.count(camera->uuid);
					if (renderStates[scene->uuid].count(camera->uuid) == 0) {
						renderState = GLRenderState::create();
						renderStates[scene->uuid].insert({ camera->uuid,renderState });
											
					}
					
					return renderStates[scene->uuid][camera->uuid];
					
				}
			}		
			

		};
	}
}
#endif
