//
// Created by rick on 2021/4/16.
//
#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <QString>
#include <QDebug>

void frameBufferSizeCallback(GLFWwindow* window,int width,int height) {
    glViewport(0,0,width,height);
}
int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (auto window =
            glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
        window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } else {
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

        // glad:load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << '\n';
            return -1;
        }

        auto  renderer = glGetString(GL_RENDER);
        auto vendor = glGetString(GL_VENDOR);
        auto version = glGetString(GL_VERSION);
        auto glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
        GLint major,minor;
        glGetIntegerv(GL_MAJOR_VERSION,&major);
        glGetIntegerv(GL_MINOR_VERSION,&minor);
//        std::cerr << vendor;
        qDebug() << QString("GL Vendor: %1\nGL Render: %2")
                        .arg((char*)vendor).arg((char*)renderer);
        qDebug() << (char*)version;
        qDebug() << major << "." << minor;
        qDebug() << "GLSL version: " << (char*)glslVersion;
    }
}