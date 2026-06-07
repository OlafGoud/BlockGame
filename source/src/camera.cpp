#include "camera2.h"
#include "glm/ext/matrix_transform.hpp"
#include "input.h"
#include <functional>


glm::mat4 FlyingCamera::getProjectionMatrix() {
  return glm::perspective(glm::radians(45.0f), (float)this->SCR_WIDTH/(float)this->SCR_HEIGHT, 0.1f, 100.0f);
}


FlyingCamera::FlyingCamera() {
  this->SCR_HEIGHT = 600;
  this->SCR_WIDTH = 800;
  this->Position = glm::vec3(0.0f, 0.0f, 3.0f);
  this->Up = glm::vec3(0.0f, 0.0f, 0.0f);
  this->Right = glm::vec3(0.0f, 0.0f, 0.0f);
  this->bindKeys();
}

glm::mat4 FlyingCamera::getViewMatrix() {
  return glm::lookAt(this->Position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  //return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void FlyingCamera::recalculateVectors() {
  

}

void FlyingCamera::bindKeys() {
  
  getInputMngr()->addKey(GLFW_KEY_W, std::bind(&FlyingCamera::processForward, this));
  getInputMngr()->addKey(GLFW_KEY_S, std::bind(&FlyingCamera::processBackward, this));
  getInputMngr()->addKey(GLFW_KEY_A, std::bind(&FlyingCamera::processLeft, this));
  getInputMngr()->addKey(GLFW_KEY_D, std::bind(&FlyingCamera::processRight, this));

}




void FlyingCamera::processForward() {
  this->Position.x += 3 * getGameData()->deltaTime;
  std::cout << "w\n";
}

void FlyingCamera::processBackward() {
  this->Position.x += -3 * getGameData()->deltaTime;
  std::cout << "s\n";
}

void FlyingCamera::processLeft() {
  this->Position.z += -3 * getGameData()->deltaTime;
  std::cout << "a\n";
}

void FlyingCamera::processRight() {
  this->Position.z += 3 * getGameData()->deltaTime;
  std::cout << "d\n";
}
