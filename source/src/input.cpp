#include "input.h"
#include "GLFW/glfw3.h"
#include <functional>

static InputManager inputManager = InputManager();
static GameData gameData = GameData();

InputManager* getInputMngr() {
  return &inputManager; 
}

GameData* getGameData() {
  return &gameData;
}

int reload = 0;

void processInput(GLFWwindow* window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    reload = 1;

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
