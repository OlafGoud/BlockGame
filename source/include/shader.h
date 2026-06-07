#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader {
public:
  Shader();
  ~Shader();
  Shader(const char* vertexPath, const char* fragmentPath);
  
  void use();

  void setBool(const std::string &name, bool value);
  void setfloat(const std::string &name, float value); 
  void setInt(const std::string &name, int value);
  void setMat4f(const std::string &name, glm::mat4 value);
  void setVec4f(const std::string &name, glm::vec4 value);
  void setVec3f(const std::string &name, glm::vec3 value);
  void setVec2f(const std::string &name, glm::vec2 value);

  void reloadShaders();
  bool isCompiled();
  void toggleLineMode();

private:
  void compileShaderProgram(const char* vertexPath, const char* fragmentPath);

  const char* vertexPath_;
  const char* fragmentPath_;
  GLuint programID;
  bool compiled;
};
