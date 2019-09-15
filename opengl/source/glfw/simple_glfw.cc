//
// Created by rick on 2019/9/6.
//

#include <GLFW/glfw3.h>
#include <iostream>

void error_callback(int code,const char* description) {
    std::cerr << description << std::endl;
}

int main(){
    if (!glfwInit()){
        std::cerr << "error"<<std::endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    auto window = glfwCreateWindow(640,480,"My Title", nullptr,nullptr);
    glfwDestroyWindow(window);

    glfwTerminate();
}