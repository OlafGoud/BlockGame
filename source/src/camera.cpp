#include "camera2.h"
#include "input.h"
#include <functional>


glm::mat4 FlyingCamera::getProjectionMatrix() {
  return glm::perspective(glm::radians(this->zoom), (float)this->SCR_WIDTH/(float)this->SCR_HEIGHT, 0.1f, 100.0f);
}


FlyingCamera::FlyingCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
  this->updateViewPort();

  this->Position = position;
  this->WorldUp = up;
  this->yaw = yaw;
  this->pitch = pitch;

  this->Front = glm::vec3(0.0f, 0.0f, -1.0f);

  glfwGetCursorPos(getGameData()->getWindow(), &this->lastX, &this->lastY);

  this->recalculateVectors();
  this->bindKeys();
}

glm::mat4 FlyingCamera::getViewMatrix() {
  return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
  
  //return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void FlyingCamera::recalculateVectors() {
  glm::vec3 front;
  front.x = std::cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = std::sin(glm::radians(pitch));
  front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  this->Front = front;
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}

void FlyingCamera::bindKeys() {
  
  getInputMngr()->addKey(GLFW_KEY_W, std::bind(&FlyingCamera::processForward, this));
  getInputMngr()->addKey(GLFW_KEY_S, std::bind(&FlyingCamera::processBackward, this));
  getInputMngr()->addKey(GLFW_KEY_A, std::bind(&FlyingCamera::processLeft, this));
  getInputMngr()->addKey(GLFW_KEY_D, std::bind(&FlyingCamera::processRight, this));
  
  getInputMngr()->addKey(KEY_MOUSE_MOVE, std::bind(&FlyingCamera::rotateCamera, this));
  getInputMngr()->addKey(KEY_MOUSE_SCROLL, std::bind(&FlyingCamera::zoomCamera, this));

  getInputMngr()->addKey(KEY_RELOAD_VIEWPORT, std::bind(&FlyingCamera::updateViewPort, this));

}

void FlyingCamera::updateViewPort() {
  glfwGetFramebufferSize(getGameData()->getWindow(), &this->SCR_WIDTH, &this->SCR_HEIGHT);
  std::cout << "width: " << this->SCR_WIDTH << "\n";
}


void FlyingCamera::processForward() {
  this->Position += this->Front * this->speed * getGameData()->deltaTime;
  std::cout << "w\n";
}

void FlyingCamera::processBackward() {
  this->Position -= this->Front * this->speed * getGameData()->deltaTime;
  std::cout << "s\n";
}

void FlyingCamera::processLeft() {
  this->Position -= this->Right * this->speed * getGameData()->deltaTime;
  std::cout << "a\n";
}

void FlyingCamera::processRight() {
  this->Position += this->Right * this->speed * getGameData()->deltaTime;
  std::cout << "d\n";
}


void FlyingCamera::rotateCamera() {
  double xPos, yPos;
  glfwGetCursorPos(getGameData()->getWindow(), &xPos, &yPos);

  float xoffset = (xPos - lastX) * this->MouseSensitivity;
  float yoffset = (lastY - yPos) * this->MouseSensitivity;

  lastX = xPos;
  lastY = yPos;

  this->yaw += xoffset;
  this->pitch += yoffset;

  if(this->pitch > 89.0f) this->pitch = 89.0f;
  if(this->pitch < -89.0f) this->pitch = -89.0f;

  recalculateVectors();

}


void FlyingCamera::zoomCamera() {
  this->zoom -= getGameData()->scrollOffset;

  if(this->zoom > 45.0f) this->zoom = 45.0f;
  if(this->zoom < 10.0f) this->zoom = 10.0f;
}
