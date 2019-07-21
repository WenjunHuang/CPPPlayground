#pragma once

#include <string>
#include <fstream>

class Shader {
public:
  Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
    std::ifstream vertexIf(vertexShaderPath);
    std::ifstream fragmentIf(fragmentShaderPath);

    if (!vertexIf.is_open())
      std::cerr << "vertex open error" << std::endl;
    if (!fragmentIf.is_open())
      std::cerr << "fragment open error " << std::endl;

    for (std::string line; std::getline(vertexIf, line);) {
      _vertexSourceCode.append(line + "\n");
    }

    for (std::string line; std::getline(fragmentIf, line);) {
      _fragmentSourceCode.append(line + "\n");
    }
  }

  const std::string &vertexSourceCode() const {
    return _vertexSourceCode;
  }

  const std::string &fragmentSourceCode() const {
    return _fragmentSourceCode;
  }

  void setBool(const std::string& name,bool value) {
    glUniform1i(glGetUniformLocation(_id,name.c_str()),(int)value);
  }

  void setInt(const std::string& name,int value) {
    glUniform1i(glGetUniformLocation(_id,name.c_str()),value);
  }

  void setFloat(const std::string& name,float value){
    glUniform1f(glGetUniformLocation(_id,name.c_str()),value);
  }

  uint32_t compile() {
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto vs = _vertexSourceCode.c_str();
    glShaderSource(vertexShader, 1, &vs, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
      std::cerr << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
      exit(-1);
    }

    //fragment shader
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fs = _fragmentSourceCode.c_str();
    glShaderSource(fragmentShader, 1, &fs, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
      std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
      exit(-1);
    }

    //link shaders
    auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
      std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
      exit(-1);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    _id = shaderProgram;
    return shaderProgram;
  }

  void use(){
    glUseProgram(_id);
  }

private:
  std::string _vertexSourceCode;
  std::string _fragmentSourceCode;
  uint32_t _id;
};