# ThreeCpp
three.js is a very powerful, simple and useful OpenGL Graphics library.
I always want to express my deep appreciation to the team that created this library.(https://github.com/mrdoob/three.js).
There was a time when I wanted to show how to use these 3D geometry and mathematical theories with OpenGL libraries. 
In that case, the examples of three.js was a very good.

However, someone who are familiar with structural languages such as C++, Java, C#, etc find it difficult to understand how three.js works.
So I decided to port three.js to C++, C#. However, it seemed that it would take too much time to port the massive examples of three.js
Fortunately, I found very simple and key examples in Learning Three.js written by Jos Dirksen(https://github.com/josdirksen/learning-threejs)
Most of three.js core have been completed, but some code have not yet been done because I am lazy. 
I don't know if I can afford it, the rest will be completed as soon as time permits.
The focus of this C++ porting job is the data loading part like as .obj, other models is also in progress.
(<a href="http://github.com/hjoykim/THREE">c# version was done for several models</a>)


## Example code

	virtual void init() override {

        renderer->shadowMap->enabled = true;
        renderer->shadowMap->type = ShadowMapType::PCFSoftShadowMap;

		    camera = std::make_shared<PerspectiveCamera>(45.0f, screenX/screenY , 0.1f, 1000.0f);
		    camera->position.set(-30.0f, 50.0f, 40.0f);
		    camera->lookAt(Vector3());

        isPerspective = true;

        Vector4 screen = Vector4(0, 0, screenX, screenY);
        controller = std::make_shared<TrackballControls>(camera, screen);

        controller->staticMoving = true;
        controller->rotateSpeed = 3.0f;
        controller->zoomSpeed = 2;
        controller->panSpeed = 2;
        controller->noZoom = false;
        controller->noPan = false;
        controller->noRotate = false;
        controller->dynamicDampingFactor = 0.2f;

        scene = std::make_shared<Scene>();
        scene->setBackgroundColor(Color().set(0xffffff));


        auto axis = std::make_shared<AxesHelper>(20);
        auto planeGeometry = std::make_shared<PlaneGeometry>(60, 20);
        auto planeMaterial = std::make_shared<MeshBasicMaterial>();
        planeMaterial->color = Color(0xcccccc);
        auto plane = std::make_shared<Mesh>(planeGeometry, planeMaterial);

        plane->rotation.setX((float)(-0.5f * M_PI));

        plane->position.set(15.0f, 0.0f, 0.0f);

        auto cubeGeometry = std::make_shared<BoxGeometry>(4, 4, 4);
        auto cubeMaterial = std::make_shared<MeshBasicMaterial>();
        cubeMaterial->color = Color(0xff0000);
        cubeMaterial->wireframe = true;
        auto cube = std::make_shared<Mesh>(cubeGeometry, cubeMaterial);
        cube->position.set(-4.0f, 3.0f, 0.0f);
        scene->add(axis);
        scene->add(plane);
        scene->add(cube);

        auto sphereGeometry = std::make_shared<SphereGeometry>(4, 20, 20);
        auto sphereMaterial = std::make_shared<MeshBasicMaterial>();
        auto sphere = std::make_shared<Mesh>(sphereGeometry, sphereMaterial);
        sphereMaterial->color = Color(0x7777ff);
        sphereMaterial->wireframe = true;

        sphere->position.set(20, 4, 2);
        scene->add(sphere);

        
	}

### Example Capture
> Camera, Scene, Light, TrackballControls, Material, OBJLoader
#### MFC Application

![image](https://user-images.githubusercontent.com/3807476/166915870-33860cda-86cd-4683-8927-09649c624749.png)

#### GLFW Application
![image](https://user-images.githubusercontent.com/3807476/166916614-7cdaf65f-1e1d-426e-bb99-fc6fccad2002.png)

