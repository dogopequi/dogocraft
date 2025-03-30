#ifndef CORE_H
#define CORE_H
#include "raylib.h"
#include "utils.h"
#define CHUNK_SIZE 16
#define CUBE_SIZE 32

#define CELL_WIDTH 16
#define CELL_HEIGHT 16
#define TEXTURES_COUNT 11
#define AIR 0
#define GRASS 1
#define STONE 2
#define DIRT 3
#define LOG 4
#define LEAVES 5
#define GLASS 6
#define SAND 7
#define WATER 8
#define PLANKS 9
#define CRAFTING_TABLE 10



typedef struct Cell{
    int column;
    int row; 
} Cell;
typedef struct Block{
    int type;
    Vector3 pos;

} Block;

typedef struct Chunk {
    Block* blocks;
    int height;

} Chunk;

Chunk* create_chunk(int height, Vector3 pos);
void draw_chunk(Chunk* chunk);
int validate_index(int index, int max, Chunk* chunk);
void validate_indexes(int left, int right, int top, int bottom, int front, int back, int max, int* canrender, Chunk* chunk);
void generateChunk(Chunk* chunk, int chunkX, int chunkZ);
Cell get_cell_from_type(int type);
RenderTexture2D get_texture_from_atlas(Cell cell);
void init_textures_array();
void free_textures_array();
extern RenderTexture2D* textures;

#endif