#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <list>
#include <unordered_map>
#include <iostream>

extern int reload;
void processInput(GLFWwindow* window);


class InputManager {
public:
  InputManager();
  void processInput(GLFWwindow* window);
  void addKey(int key, std::function<void()> function);
  void removeKey(int key, std::function<void()> function);
  
  void exitProgram();
  
private:
  std::unordered_map<int, std::list<std::function<void()>>> keyFunctions{};

};

InputManager* getInputMngr();


class GameData {
public:
  void setWindow(GLFWwindow* window);
  GLFWwindow* getWindow();

  float deltaTime;
private:
  GLFWwindow* window;
};

GameData* getGameData();
