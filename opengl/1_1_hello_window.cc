//
// Created by rick on 2020/2/15.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void frameBufferSizeCallback(GLFWwindow*,int, int);
void processInput(GLFWwindow*);

int main(int argc, char* argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    if (auto window =
            glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
        window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } else {
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window,frameBufferSizeCallback);

        // glad:load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << '\n';
            return -1;
        }

        // render loop
        while(!glfwWindowShouldClose(window)) {
            // input
            processInput(window);

            // render
            glClearColor(0.2f,0.3f,0.3f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //glfw: swap buffers and poll IO events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);
}

void frameBufferSizeCallback(GLFWwindow* window,int width,int height) {
    glViewport(0,0,width,height);
}