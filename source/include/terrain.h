#include <array>
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
};

void generateChunk(std::array<BlockType, CHUNK_SIZE_3> &chunkArray);
void generateChunkFaces(const std::array<BlockType, CHUNK_SIZE_3> &chunkArray, std::vector<float> &vertices);

