#include "terrain.h"
#include <cstdlib>
#include <glm/glm.hpp>

static const glm::vec3 FACE_VERTS[6][4] = {
  // +X
  {{1,0,0},{1,1,0},{1,1,1},{1,0,1}},
  // -X
  {{0,0,0},{0,0,1},{0,1,1},{0,1,0}},
  // +Y
  {{0,1,0},{0,1,1},{1,1,1},{1,1,0}},
  // -Y
  {{0,0,0},{1,0,0},{1,0,1},{0,0,1}},
  // +Z
  {{0,0,1},{1,0,1},{1,1,1},{0,1,1}},
  // -Z
  {{0,0,0},{0,1,0},{1,1,0},{1,0,0}},
};

Chunk::Chunk() {
  for(int z = 0; z < CHUNK_SIZE; z++) for(int y = 0; y < CHUNK_SIZE; y++) for(int x = 0; x < CHUNK_SIZE; x++) {
    this->blocks[x][y][z].state = 0;
    if(rand()% 4 < 1) {
      this->blocks[x][y][z].type = GRASS;
    } else if (rand() %6 < 2) {
      this->blocks[x][y][z].type = ORANGE;
    } else {
      this->blocks[x][y][z].type = DIRT;
    }
  }

  this->recalculateChunk();
}

Chunk::~Chunk() {
  glDeleteVertexArrays(1, &this->VAO);
  glDeleteBuffers(1, &this->VBO);
  glDeleteBuffers(1, &this->EBO);
}

void Chunk::recalculateChunk() {
  vertices.clear();
  indices.clear();
  for(int z = 0; z < CHUNK_SIZE; z++) for(int y = 0; y < CHUNK_SIZE; y++) for(int x = 0; x < CHUNK_SIZE; x++) {
    

    /** each face */
    for(int f = 0; f < 6; f++) {
      /** check for if it needs to be renderd */
      unsigned int base = vertices.size() / 7;
      for(int i = 0; i < 4; i++) {
        vertices.push_back(x + FACE_VERTS[f][i].x);
        vertices.push_back(y + FACE_VERTS[f][i].y);
        vertices.push_back(z + FACE_VERTS[f][i].z);

        if(this->blocks[x][y][z].type == ORANGE) {
          vertices.push_back(1.0f);
          vertices.push_back(0.5f);
          vertices.push_back(0.2f);
          vertices.push_back(1.0f);
        } else if (this->blocks[x][y][z].type == GRASS) {
          vertices.push_back(0.2f);
          vertices.push_back(1.0f);
          vertices.push_back(0.5f);
          vertices.push_back(0.8f);
        } else {
          vertices.push_back(0.2f);
          vertices.push_back(0.5f);
          vertices.push_back(1.0f);
          vertices.push_back(0.8f);         
        }


      }
      
      indices.push_back(base + 0);
      indices.push_back(base + 1);
      indices.push_back(base + 2);

      indices.push_back(base + 0);
      indices.push_back(base + 2);
      indices.push_back(base + 3);
    }
  }
  this->updateGPUBuffers();
}

void Chunk::changeBlock() {
}

void Chunk::render() {
  glBindVertexArray(this->VAO);
  glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Chunk::updateGPUBuffers() {
  glDeleteVertexArrays(1, &this->VAO);
  glDeleteBuffers(1, &this->VBO);
  glDeleteBuffers(1, &this->EBO);
  
  glGenVertexArrays(1, &this->VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &this->VBO);
  glGenBuffers(1, &this->EBO);

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), this->indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  
  glBindVertexArray(0);
}








