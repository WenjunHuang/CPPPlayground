#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

static void glfwErrorCallback(int error,const char* description){
    std::cerr << "Glfw Error " << error << ":" << description << std::endl;
}

int main(int argc,char *argv[]) {

    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
        return 1;
#ifdef __APPLE__
    // GL 3.2 + GLSL 150
    const char* glslVersion = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    auto window = glfwCreateWindow(1280,720,"Dear ImGui GLFW+OpenGL3 example",nullptr,nullptr);
    if (window ==nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    bool err = glewInit() != 0;

    if (err) {
        std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();(void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    auto showDemoWindow = true;
    auto showAnotherWindow = false;
    auto clearColor = ImVec4(0.45f,0.55f,0.60f,1.0f);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");

            ImGui::Text("This is some useful text.");
            ImGui::Checkbox("Demo Window",&showDemoWindow);
            ImGui::Checkbox("Another Window",&showAnotherWindow);

            ImGui::SliderFloat("float",&f,0.0f,1.0f);
            ImGui::ColorEdit3("clear color",(float*)&clearColor);
            if (ImGui::Button("Button"))
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d",counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);

            ImGui::End();
        }

        if (showAnotherWindow){
            ImGui::Begin("Another Window",&showAnotherWindow);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                showAnotherWindow = false;
            ImGui::End();
        }
        ImGui::Render();

        int displayW,displayH;
        glfwGetFramebufferSize(window,&displayW,&displayH);
        glViewport(0,0,displayW,displayH);
        glClearColor(clearColor.x,clearColor.y,clearColor.z,clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();



}