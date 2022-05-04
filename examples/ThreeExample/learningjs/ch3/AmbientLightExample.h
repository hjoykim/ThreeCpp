#ifndef AMBIENTLIGHT_EXAMPLE_H
#define AMBIENTLIGHT_EXAMPLE_H
#include <ecore/ThreeDemoModel.h>
#include <ecore/ExampleUtil.h>
#include <imgui\imgui.h>

class AmbientLightExample : public ThreeDemoClass {
public:
	AmbientLight::ptr ambientLight;
	SpotLight::ptr spotLight0;
	ImVec4 iColor;
	
	AmbientLightExample(int x, int y);
	
	
	virtual void init() override;

	virtual void render() override;

	virtual void showControls() override;
};
#endif