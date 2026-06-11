#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "input.h"
#include "shader.h"
#include "camera2.h"
#include "terrain.h"


int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to load window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, frameBuffersizeCallback);
  glfwSetCursorPosCallback(window, mouseMoveCallback);
  glfwSetScrollCallback(window, mouseScrollCallback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
    std::cout<<"Failed GLAD\n"; 
    return -1;
  }

  getGameData()->setWindow(window);

  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << "\n";

  glViewport(0, 0, 800, 600);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND); /* text */
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); /* text*/

  Shader shader = Shader("build/resources/shaders/basic.vs", "build/resources/shaders/basic.fs");
  FlyingCamera cam = FlyingCamera(glm::vec3(0.0f, 200.0f, 0.0f));


  getChunkMngr()->init(&shader);
  unsigned int frameCounter = 0;
  float timeLast = 0;
  float lastFrame = static_cast<float>(glfwGetTime()); 
  while(!glfwWindowShouldClose(window)) { 
    frameCounter++;
    float currentFrame = static_cast<float>(glfwGetTime()); 
    getGameData()->deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if(timeLast + 1 < currentFrame) {
      std::cout << "FPS: " << frameCounter / (currentFrame - timeLast) << "\n";
      timeLast = currentFrame;
      frameCounter = 0;
    }
    getInputMngr()->processInput(window);
    

    glClearColor(0.1f,0.1f,0.1f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();

    glm::mat4 projection = cam.getProjectionMatrix();
    glm::mat4 view = cam.getViewMatrix();
    getChunkMngr()->renderChunks(projection, view); 

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;

}
