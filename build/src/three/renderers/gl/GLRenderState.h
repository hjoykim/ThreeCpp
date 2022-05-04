//#ifndef GLRENDER_STATE_THREE_H
//#define GLRENDER_STATE_THREE_H
//#include <three/cameras/Camera.h>
//#include <three/lights/Light.h>
//#include <three/renderers/gl/GLLights.h>
//namespace three {
//	namespace gl {
//		class GLRenderState
//		{
//		public:
//			struct RenderState {
//				std::vector<Light::ptr> lightsArray;
//				std::vector<Light::ptr> shadowsArray;
//				GLLights lights;
//			};
//
//			using ptr = std::shared_ptr<GLRenderState>;
//
//			RenderState state;
//
//			GLRenderState() {}
//
//			using ptr = std::shared_ptr<GLRenderState>;
//
//			static ptr create() {
//				return ptr(new GLRenderState());
//			}
//			virtual ~GLRenderState() = default;
//
//			void init() {
//				state.lightsArray.clear();
//				state.shadowsArray.clear();
//			}
//
//			void pushLight(Light::ptr& light) {
//				state.lightsArray.push_back(light);
//			}
//			void pushShadow(Light::ptr& shadowLight) {
//				state.shadowsArray.push_back(shadowLight);
//			}
//			void setupLights(Camera::ptr& camera) {
//				state.lights.setup(state.lightsArray, *camera);
//			}
//		};		
//	}
//}
//#endif
