#ifndef THREE_DEMO_MODEL_H
#define THREE_DEMO_MODEL_H
#include <three/core/BufferAttribute.h>
#include <three/core/BufferGeometry.h>
#include <three/renderers/GLRenderer.h>
#include <three/geometries/PlaneGeometry.h>
#include <three/geometries/SphereGeometry.h>
#include <three/geometries/BoxGeometry.h>
#include <three/renderers/gl/GLUniforms.h>
#include <three/scenes/Scene.h>
#include <three/cameras/Camera.h>
#include <three/cameras/PerspectiveCamera.h>
#include <three/cameras/OrthographicCamera.h>
#include <three/controls/TrackballControls.h>
#include <three/renderers/GLRenderer.h>
#include <memory>
using namespace three;
using namespace three::gl;
using namespace three::control;
class ThreeDemoClass {
public:
	using ptr = std::shared_ptr<ThreeDemoClass>;
	bool initialized = false;
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<PerspectiveCamera> pcamera;
	std::shared_ptr<OrthographicCamera> ocamera;
	std::shared_ptr<TrackballControls> controller;
	std::shared_ptr<GLRenderer> renderer;
	int screenX;
	int screenY;
	bool isPerspective;
	ThreeDemoClass(int x,int y) :screenX(x),screenY(y) {
		renderer = std::make_shared<GLRenderer>(x, y);
		//renderer->debug.add("checkShaderErrors", true);
		init();
	}

	virtual void init(){}
	virtual void showControls() {}
	virtual void render() {
		
		renderer->render(scene, camera);
		
	}
};
#endif