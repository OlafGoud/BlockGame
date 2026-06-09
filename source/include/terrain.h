#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <unordered_map>

#include "shader.h"
#define CHUNK_SIZE 16 
#define CHUNK_Y_SIZE 64 
#define CHUNK_SIZE_3 CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

enum BlockType {
  AIR,
  GRASS,
  DIRT,
  ROAD,
  ORANGE
};


struct Block {
  BlockType type;
  int state;
};

class Chunk {
public:

  Chunk();
  ~Chunk();

  void recalculateChunk();
  void changeBlock();
  void render();

private:
  Block blocks[CHUNK_SIZE][CHUNK_Y_SIZE][CHUNK_SIZE]{};
  
  GLuint VAO, VBO, EBO;
  std::vector<float> vertices{};
  std::vector<unsigned int> indices{};
  bool dirty = true;
  void updateGPUBuffers();
};


class ChunkManager {
public:
  ChunkManager();
  ~ChunkManager();

  void init(Shader* shader);

  void renderChunks(glm::mat4 proj, glm::mat4 view);
  void loadChunk(int x, int z);



  
  
private:
  Shader* shader;
  std::unordered_map<uint64_t, std::unique_ptr<Chunk>> loadedChunks;  

  uint64_t createKey(int x, int z);
  std::pair<int, int> decodeKey(uint64_t key);
};

ChunkManager* getChunkMngr();

