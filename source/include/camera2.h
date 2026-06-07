#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class FlyingCamera {
public:
  FlyingCamera();
  
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();

  void processForward();
  void processBackward();
  void processLeft();
  void processRight();
  void bindKeys();


  
  void updateViewPort();

  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  






private:
  int SCR_WIDTH = 0;
  int SCR_HEIGHT = 0;
  float yaw = -90.0f;
  float pitch = 0.0f;

  float speed = 2.5f;
  float sensitivity = 0.1f;
  float zoom = 45.0f;



  void recalculateVectors();
};
