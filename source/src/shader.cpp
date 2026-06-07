#include "shader.h"
#include "GLFW/glfw3.h"

#include <ctime>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <input.h>


Shader::Shader() {
  std::cout << "Empty shader\n";
  this->compiled = false;
  this->fragmentPath_ = "\0";
  this->vertexPath_ = "\0";
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  this->vertexPath_ = vertexPath;
  this->fragmentPath_ = fragmentPath;
  compileShaderProgram(vertexPath, fragmentPath);
  getInputMngr()->addKey(GLFW_KEY_R, std::bind(&Shader::reloadShaders, this));
  getInputMngr()->addKey(GLFW_KEY_P, std::bind(&Shader::toggleLineMode, this));
}

Shader::~Shader() {
  glDeleteProgram(this->programID); 
  std::cout << "Shader destroyed\n";
}

void Shader::compileShaderProgram(const char* vertexPath, const char* fragmentPath) {
  this->compiled = true;
  std::string vertexCode;
  std::string fragmentCode;

  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch(std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE::FILE_NOT_READED_OR_FOUND\n";
    this->compiled = false;
    return;
  }

  const char* vCode = vertexCode.c_str();
  const char* fCode = fragmentCode.c_str();
  GLuint vertex, fragment;
  int successv, successf;
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vCode, NULL);
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &successv);
  if(!successv) {
    char infoLog[512];
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    compiled = false;
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fCode, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &successf);
  if(!successf) {
    char infoLog[512];
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    compiled = false;
  }

  this->programID = glCreateProgram();
  glAttachShader(this->programID, vertex);
  glAttachShader(this->programID, fragment);
  glLinkProgram(this->programID);

  int successl;
  glGetProgramiv(this->programID, GL_LINK_STATUS, &successl);
  if(!successl) {
    char infoLog[512];
    glGetShaderInfoLog(this->programID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::LINKING::LINKING_FAILED\n" << infoLog << std::endl;
    compiled = false;
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}


void Shader::use() {
  if(compiled == false) return;

  glUseProgram(this->programID);
}

bool Shader::isCompiled() {
  return this->compiled;
}

void Shader::reloadShaders() {
  this->compileShaderProgram(this->vertexPath_, this->fragmentPath_);
  std::cout << "recompiled shaders!\n";
}

void Shader::toggleLineMode() {
  static bool toggle = false;
  static float lastTime = 0;

  if(lastTime + 0.2f > static_cast<float>(glfwGetTime())) return;

  if(toggle == false) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  toggle = !toggle;
  lastTime = static_cast<float>(glfwGetTime());
}


/** Uniform functions */
void Shader::setBool(const std::string &name, bool value) {
  glUniform1i(glGetUniformLocation(this->programID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) {
  glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Shader::setfloat(const std::string &name, float value) {
  glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Shader::setMat4f(const std::string &name, glm::mat4 value) {
  glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setVec4f(const std::string &name, glm::vec4 value) {
  glUniform4fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3f(const std::string &name, glm::vec3 value) {
  glUniform3fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2f(const std::string &name, glm::vec2 value) {
  glUniform2fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}










