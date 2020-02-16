#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void frameBufferSizeCallback(GLFWwindow*,int, int);
void processInput(GLFWwindow*);

const char* vertexShaderSource = R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);
}
)";
const char* fragmentShaderSource = R"(#version 330 core
out vec4 FragColor;
uniform vec4 ourColor;
void main(){
//    FragColor = vec4(1.0f,0.5f,0.2f,1.0f);
    FragColor = ourColor;
}
)";

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


        // build and compile our shader program
        // vertex shader
        auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource,nullptr);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
        if (!success) {
            glGetShaderInfoLog(vertexShader,512,nullptr,infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // fragment shader
        auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,&fragmentShaderSource,nullptr);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader,512,nullptr,infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // link shaders
        auto shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram,vertexShader);
        glAttachShader(shaderProgram,fragmentShader);
        glLinkProgram(shaderProgram);

        // check for linking errors
        glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram,512,nullptr,infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog<< std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        float vertices[] = {
            -0.9f,-0.5f,-0.0f, // left
            0.0f,-0.5f,0.0f, // right
            -0.45f,0.5f,0.0f, // top

            // second triangle
            0.0f,-0.5f,0.0f,
            0.9f,-0.5f,0.0f,
            0.45f,0.5f,0.0f
        };
        unsigned int VBO,VAO;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s),and then configure vertex attribute(s)
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);

        // render loop
        while(!glfwWindowShouldClose(window)) {
            // input
            processInput(window);

            // render
            glClearColor(0.2f,0.3f,0.3f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw our first triangle
            float timeValue = glfwGetTime();
            float greenValue = (std::sin(timeValue)/2.0f) + 0.5f;
            auto vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");

            glUseProgram(shaderProgram);
            glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES,0,6);

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