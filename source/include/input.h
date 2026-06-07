#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <list>
#include <unordered_map>
#include <iostream>

#define KEY_MOUSE_MOVE 1000
#define KEY_MOUSE_SCROLL 1001
#define KEY_RELOAD_VIEWPORT 1002


extern int reload;
void processInput(GLFWwindow* window);
void frameBuffersizeCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

class InputManager {
public:
  InputManager();
  void processInput(GLFWwindow* window);
  void addKey(int key, std::function<void()> function);
  void removeKey(int key, std::function<void()> function);
  void executeKey(int key);
  
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
  float scrollOffset;
private:
  GLFWwindow* window;
};

GameData* getGameData();
