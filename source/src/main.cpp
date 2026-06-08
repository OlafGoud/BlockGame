#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <vector>
#include <array>
#include "glm/ext/matrix_transform.hpp"
#include "input.h"
#include "shader.h"
#include "camera2.h"
#include "terrain.h"

float verticesold[] = {
  0.5f,  0.5f, 0.0f,  // top right
  0.5f, -0.5f, 0.0f,  // bottom right
  -0.5f,  0.5f, 0.0f,  // top left 
    // second triangle
  0.5f, -0.5f, 0.0f,  // bottom right
  -0.5f, -0.5f, 0.0f,  // bottom left
  -0.5f,  0.5f, 0.0f   // top left
};

unsigned int indices[] = {
  0, 1, 3,
  1, 2, 3
};

float verticesTriangle[] = {
  -0.5f, -0.5f, 0.0f,
  0.5, -0.5f, 0.0f,
  0.0f, 0.5f, 0.0f
};


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
  FlyingCamera cam = FlyingCamera(glm::vec3(0.0f, 0.0f, 3.0f));
  std::cout << "shader\n";

  Chunk chunk = Chunk();

  GLuint VBO, VAO, IDC;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &IDC);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesold), verticesold, GL_STATIC_DRAW);
  /*
  glBindBuffer(GL_ARRAY_BUFFER, IDC);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(int), positions.data(), GL_STATIC_DRAW);
  */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  /*
  glVertexAttribPointer(1, 3, GL_INT, GL_FALSE, 3 * sizeof(int) + 3 * sizeof(int), (void*)(3 * sizeof(int)));
  glEnableVertexAttribArray(1);
  */
  glBindVertexArray(0);

  float timeLast = 0;
  float lastFrame = static_cast<float>(glfwGetTime()); 
  while(!glfwWindowShouldClose(window)) { 

    float currentFrame = static_cast<float>(glfwGetTime()); 
    getGameData()->deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if(timeLast + 1 < currentFrame) {
      timeLast = currentFrame;
      std::cout << "FPS: " << 1/getGameData()->deltaTime << "\n";
    }
    getInputMngr()->processInput(window);


    glClearColor(0.1f,0.1f,0.1f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();

    glm::mat4 projection = cam.getProjectionMatrix();
    glm::mat4 view = cam.getViewMatrix();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    shader.setMat4f("proj", projection);
    shader.setMat4f("view", view);
    shader.setMat4f("model", model);
    chunk.render();
    /*glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); 
    glBindVertexArray(0);
*///

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;

}
