#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#define CHUNK_SIZE 16 
#define CHUNK_SIZE_3 CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

enum BlockType {
  AIR,
  GRASS,
  DIRT,
  ROAD
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
  Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]{};
  
  GLuint VAO, VBO, EBO;
  std::vector<float> vertices{};
  std::vector<int> indices{};
  bool dirty = true;

  void updateGPUBuffers();
};



