
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <three/helpers/AxesHelper.h>
#include <three/controls/TrackballControls.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <three/renderers/gl/GLState.h>
#include <stdio.h>


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// settings
int display_w=1102;
int display_h=999;

GLFWwindow* window;



static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#include <ecore/InitApplication.h>


ImGuiIO* demoIO;

int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); // 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
// --------------------
    window = glfwCreateWindow(1280,720, "THREE Example", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetScrollCallback(window, mouse_scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    demoIO = &io;
    ImGui::StyleColorsDark();

    const char* glsl_version = "#version 460";
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 16.0f);
    io.Fonts->AddFontFromFileTTF("Cousine-Regular.ttf", 15.0f);
    io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 16.0f);
    io.Fonts->AddFontFromFileTTF("ProggyTiny.ttf", 10.0f);

   
    //GLRenderer renderer;

    initRenderer();

    glfwGetFramebufferSize(window, &display_w, &display_h);
    framebuffer_size_callback(window, display_w, display_h);

    bool show_demo_window = true;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    //std::unordered_map<int, three::gl::GLState::BoundTexture> currentBoundTextures;
    while (!glfwWindowShouldClose(window))
    {
        if (currentDemoClass != nullptr) {

            if (!currentDemoClass->initialized) {
                Color c = currentDemoClass->renderer->getClearColor();
                glClearColor(c.r, c.g, c.b,1.0f);
                currentDemoClass->renderer->clear(true, true, true);
                currentDemoClass->init();
                glfwGetFramebufferSize(window, &display_w, &display_h);
                framebuffer_size_callback(window, display_w, display_h);
                currentDemoClass->initialized = true;
            }

            if (!io.WantCaptureMouse) {
                currentDemoClass->controller->update();    
            }
            //currentDemoClass->renderer->state->currentBoundTextures = currentBoundTextures;
            currentDemoClass->render();
            
        }
       
        // input
        // -----
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ShowApplicationMenuBar();

        processInput(window);
        
        if (currentDemoClass != nullptr) {
            currentDemoClass->showControls();
            // save boundTextures before ImGui Render
            //currentBoundTextures = currentDemoClass->renderer->state->currentBoundTextures;
        }
        //{
        //    static float f = 0.0f;
        //    static int counter = 0;

        //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //    ImGui::Checkbox("Another Window", &show_another_window);

        //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //        counter++;
        //    ImGui::SameLine();
        //    ImGui::Text("counter = %d", counter);

        //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //    ImGui::End();
        //}
        // 3. Show another simple window.
        //if (show_another_window)
        //{
        //    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //    ImGui::Text("Hello from another window!");
        //    if (ImGui::Button("Close Me"))
        //        show_another_window = false;
        //    ImGui::End();
        //}

       
        // Rendering
        ImGui::Render();
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

       
        // render
        // ------
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        //glUseProgram(shaderProgram);
        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------       
        glfwSwapBuffers(window); 
        
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void processInput(GLFWwindow* window)
{
   /* if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);*/
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    //glViewport(0, 0, width, height);
    display_w = width;
    display_h = height;
    if (currentDemoClass == nullptr) return;
    currentDemoClass->controller->sizeChanged(Vector4(0, 0, width, height));

    if (currentDemoClass != nullptr)
        currentDemoClass->renderer->setViewport(0, 0, display_w, display_h);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (currentDemoClass == nullptr) return;
    currentDemoClass->controller->mouseMove((int)xpos, (int)ypos);
}
void mouse_button_callback(GLFWwindow* window, int button,int action,int modes) {

    if (currentDemoClass == nullptr) return;
    if (demoIO->WantCaptureMouse) return;
    bool lbutton_down=false;
    bool rbutton_down = false;
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT :
        if (GLFW_PRESS == action)
            lbutton_down = true;
        else
            lbutton_down = false;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT :
        if (GLFW_PRESS == action)
            rbutton_down = true;
        else
            rbutton_down = false;
        break;
    }
    
    if (lbutton_down || rbutton_down) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (lbutton_down) {
            currentDemoClass->controller->mouseDown(0, (int)x, (int)y);//ROTATE
        }
        else
            currentDemoClass->controller->mouseDown(2, (int)x, (int)y); // PAN

    }
    else {
        currentDemoClass->controller->mouseUp();
    }   
}
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{   
    if (currentDemoClass == nullptr) return;
    currentDemoClass->controller->mouseWheel((float)(yoffset*120));
}