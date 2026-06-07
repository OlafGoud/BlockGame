#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glm/ext/matrix_transform.hpp"
#include "input.h"
#include "shader.h"
#include "camera2.h"

float vertices[] = {
  0.5f, 0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f,
  -0.5f, 0.5f, 0.0f
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
  FlyingCamera cam = FlyingCamera();
  std::cout << "shader\n";

  GLuint VBO, VAO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  float lastFrame = static_cast<float>(glfwGetTime()); 
  while(!glfwWindowShouldClose(window)) { 

    if(reload == 1) {
      reload = 0;
      shader.reloadShaders();
    }
    float currentFrame = static_cast<float>(glfwGetTime()); 
    getGameData()->deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //std::cout << 1/deltaTime << "\n";
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

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;

}
