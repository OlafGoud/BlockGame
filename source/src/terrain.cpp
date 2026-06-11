#include "terrain.h"
#include "glm/ext/matrix_transform.hpp"
#include <cstdint>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

static ChunkManager manager = ChunkManager();
ChunkManager* getChunkMngr() {
  return &manager;
}

unsigned long int count = 0;

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

static const glm::vec3 faceNormals[6] {
  {1,0,0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
};


Chunk::Chunk(int x, int z) {
  this->x = x;
  this->z = z;

  for(int z = 0; z < CHUNK_SIZE; z++) for(int y = 0; y < CHUNK_Y_SIZE; y++) for(int x = 0; x < CHUNK_SIZE; x++) {
    this->blocks[x][y][z].state = 0;
    if(rand()% 4 < 1) {
      this->blocks[x][y][z].type = GRASS;
    } else if (rand() % 6 < 2) {
      this->blocks[x][y][z].type = ORANGE;
    } else {
      this->blocks[x][y][z].type = DIRT;
    }
  }

  //this->recalculateChunk();
}

Chunk::~Chunk() {
  glDeleteVertexArrays(1, &this->VAO);
  glDeleteBuffers(1, &this->VBO);
  glDeleteBuffers(1, &this->EBO);
}

void Chunk::recalculateChunk() {
  vertices.clear();
  indices.clear();
  for(int z = 0; z < CHUNK_SIZE; z++) for(int y = 0; y < CHUNK_Y_SIZE; y++) for(int x = 0; x < CHUNK_SIZE; x++) {
    if (!isSolidBlock(x, y, z)) continue;
    /** each face */
    for(int f = 0; f < 6; f++) {
      /** check for if it needs to be renderd */
      int xn = x + faceNormals[f].x;
      int yn = y + faceNormals[f].y;
      int zn = z + faceNormals[f].z;

      if(isSolidBlock(xn, yn, zn)) continue;
      /** check for x out of bounds */
      if(isOutOfChunk(xn, 0, CHUNK_SIZE)) {
        if(xn < 0) {
          if(getChunkMngr()->isSolidInChunk(this->x - 1, this->z, 15, yn, zn)) continue; 
        } else if (xn >= CHUNK_SIZE) {
          if(getChunkMngr()->isSolidInChunk(this->x + 1, this->z, 0, yn, zn)) continue;
        } else {
          std::cout << "ERROR::RECALC::NOTOUTOFCHUNK: n=" << xn << "\n";
        }
      }
      
      /** check for z out of bounds */
      if(isOutOfChunk(zn, 0, CHUNK_SIZE)) {
        if(zn < 0) {
          if(getChunkMngr()->isSolidInChunk(this->x, this->z - 1, xn, yn, 15)) continue;
        } else if (zn >= CHUNK_SIZE) {
          if(getChunkMngr()->isSolidInChunk(this->x, this->z + 1, xn, yn, 0)) continue;
        }
      }

      count += 2;
      
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
  std::cout << "current size: " << count << " triangles\n"; 
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

bool Chunk::isSolidBlock(int x, int y, int z) {
  if(isOutOfChunk(x, 0, CHUNK_SIZE) || isOutOfChunk(y, 0, CHUNK_Y_SIZE) || isOutOfChunk(z, 0, CHUNK_SIZE)) return false;

  Block* b = &this->blocks[x][y][z];
  return (b->state == AIR || b->state == WATER);
}

bool Chunk::isOutOfChunk(int n, int min, int max) {
  return !(n >= min && n < max);  
}




















ChunkManager::ChunkManager() {
}

void ChunkManager::init(Shader* shader) {
  this->shader = shader;

  for(int x = -10; x < 21; x++) for(int z = -10; z < 21; z++) {
    this->loadChunk(x, z);
  }
  for(auto& [key, chunk] : this->loadedChunks) {
    chunk->recalculateChunk();
  }

}


void ChunkManager::renderChunks(glm::mat4 proj, glm::mat4 view) {
  this->shader->setMat4f("proj", proj);
  this->shader->setMat4f("view", view);
  
  for(auto& [key, chunk] : this->loadedChunks) {
    std::pair<int, int> chunkCoords = this->decodeKey(key);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float>(chunkCoords.first) * CHUNK_SIZE, 0.0f, static_cast<float>(chunkCoords.second) * CHUNK_SIZE));
    this->shader->setMat4f("model", model);
    chunk->render();
  }
  
}

void ChunkManager::loadChunk(int x, int z) {
  uint64_t key = createKey(x, z);
  loadedChunks.emplace(key, std::make_unique<Chunk>(x, z));
}

uint64_t ChunkManager::createKey(int x, int z) {
  return ((uint64_t)(uint32_t)x << 32) | (uint32_t)z;
}

std::pair<int, int> ChunkManager::decodeKey(uint64_t key) {
  return {static_cast<int>(key >> 32), static_cast<int>(key)};
}

ChunkManager::~ChunkManager() {
  loadedChunks.clear();
  std::cout << "cleared chunkmanager\n";
}


bool ChunkManager::isSolidInChunk(int chunkx, int chunkz, int x, int y, int z) {
  uint64_t key = createKey(chunkx, chunkz);
  if(!loadedChunks.contains(key)) return false;

  return loadedChunks.at(key)->isSolidBlock(x, y, z);
}
