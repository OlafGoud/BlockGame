#include "input.h"
#include "GLFW/glfw3.h"
#include <functional>

static InputManager inputManager = InputManager();
static GameData gameData = GameData();
void frameBuffersizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  getInputMngr()->executeKey(KEY_RELOAD_VIEWPORT); 
}

void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos) {
  getInputMngr()->executeKey(KEY_MOUSE_MOVE);
}
void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  getGameData()->scrollOffset = static_cast<float>(yOffset);
  getInputMngr()->executeKey(KEY_MOUSE_SCROLL);
}



InputManager* getInputMngr() {
  return &inputManager; 
}

GameData* getGameData() {
  return &gameData;
}

void InputManager::processInput(GLFWwindow *window) {
  
  for(auto [key, value] : this->keyFunctions) {
    if(glfwGetKey(window, key) == GLFW_PRESS) {
      for(std::function<void()> func : value) {
        func();
      }
    }
  }
}

void InputManager::executeKey(int key) {
  for(std::function<void()> func : this->keyFunctions[key]) {
    func();
  }
}


void InputManager::addKey(int key, std::function<void()> function) {
  this->keyFunctions[key].push_back(function);
}

void InputManager::removeKey(int key, std::function<void()> function) {
  if(this->keyFunctions[key].empty()) return;
  //this->keyFunctions[key].remove(function);
}

InputManager::InputManager() {
  this->addKey(GLFW_KEY_ESCAPE, std::bind(&InputManager::exitProgram, this));

}

void InputManager::exitProgram() {
  glfwSetWindowShouldClose(getGameData()->getWindow(), true);
  std::cout << "windowshouldClose\n";
}

void GameData::setWindow(GLFWwindow* window) {
  this->window = window;
}

GLFWwindow* GameData::getWindow() {
  return this->window;
}
