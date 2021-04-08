//
// Created by rick on 2020/2/16.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int, int);
void processInput(GLFWwindow*);

constexpr unsigned int kScrWidth  = 800;
constexpr unsigned int kScrHeight = 600;

const char* vertexShaderSource = R"(
#version 330 ParentChildRelationship
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 ourColor;
uniform float xOffset;
void main() {
    gl_Position = vec4(aPos.x + xOffset,aPos.y,aPos.z,1.0);
    ourColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 ParentChildRelationship
out vec4 FragColor;
in vec3 ourColor;
void main() {
    FragColor = vec4(ourColor,1.0f);
}
)";

int main() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    if (auto window = glfwCreateWindow(kScrWidth, kScrHeight, "LearnOpenGL",
                                       nullptr, nullptr)) {
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        // shader program
        auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        auto shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        float vertices[] = {
            // positions        // colors
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
            0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
        };

        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // bind the Vertex Array Object first
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glUseProgram(shaderProgram);
        while (!glfwWindowShouldClose(window)) {
            processInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            auto vertexOffsetLocation = glGetUniformLocation(shaderProgram,"xOffset");
            glUniform1f(vertexOffsetLocation,0.5f);

            // render the triangle
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        glfwTerminate();
        return 0;
    } else {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow* window,int width,int height) {
    glViewport(0,0,width,height);
}
